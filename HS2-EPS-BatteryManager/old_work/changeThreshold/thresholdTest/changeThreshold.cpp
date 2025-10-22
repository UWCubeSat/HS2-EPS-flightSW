// This code can change the threshold value for BQ25756
// Values:
// Vfb_reg
// Vrechg
// Vbattlowv
// Vbat_short
// ICHG: charge current limit
// Ibat_short
//From datasheet, looks like we can also control
// Charging region behavior (p48), REG0x1C
// Reverse Mode(Discharge) 0x19
// JEITA warm and cool: control charging based on temperature
// REG0x1B_TS_Charging_Threshold_Control: set the temperature for JEITA
// Watch dog: monitor the system and reset IC for safety, fault issues

// Task for this code
// Temperature setings
#include "BQ25756_ard.h"
#include <Wire.h>

// Change IAC_DPM_limit
int setIAC_DPM_lim (int value) {
    if (value < 400 || value > 20000) {
        return -1;  // Invalid value, return error code
    }

    // Calculate the IAC_DPM register value (bit step is 50mA)
    uint16_t iac_dpm_value = (uint16_t)value / 50;  // Convert mA to the corresponding register value (0-400)

    // Read the current value of the register (lower and higher byte)
    uint8_t low_byte = read8BitRegister(0x06);  // Read lower byte of REG0x06
    uint8_t high_byte = read8BitRegister(0x07); // Read higher byte of REG0x06

    // Combine the high and low byte into a 16-bit value
    uint16_t current_value = (high_byte << 8) | low_byte;
    // Clear the bits 10-2 (IAC_DPM) using a mask
    current_value &= ~(0xFF << 2);  // Mask for bits 10-2

    // Set the new IAC_DPM value in bits 10-2
    current_value |= (iac_dpm_value << 2);  // Place the IAC_DPM value into bits 10-2

    // Write the updated value back to the register (lower and higher byte)
    writeRegister(0x06, current_value & 0xFF); // Write lower byte
    writeRegister(0x07, (current_value >> 8) & 0xFF); // Write higher byte

    // Read the register again to verify the actual value set
    low_byte = read8BitRegister(0x06);  // Read lower byte of REG0x06
    high_byte = read8BitRegister(0x07); // Read higher byte of REG0x06

    // Combine the high and low byte again to get the updated register value
    current_value = (high_byte << 8) | low_byte;

    // Extract the IAC_DPM value from the register (bits 10-2)
    iac_dpm_value = (current_value >> 2) & 0xFF;  // Get the 9-bit IAC_DPM value

    // Convert the register value back to milliamps (since bit step is 50mA)
    int actual_value = (int)iac_dpm_value * 50;  // Multiply by 50mA (bit step)

    // Return the actual value set
    return actual_value;
}

// Change Vfb_reg, value should be in range
// Range: 1504mV-1566mV, Bit step: 2mV, Offset: 1504mV
int setVfb_reg (int value){
    if (value < 1504 || value > 1566) {
        return -1;
    }
    uint8_t writeVal = (uint8_t)((value - 1504)/2);
    uint8_t VFB_REG = CHARGE_VOLT_LIM;
    writeRegister(CHARGE_VOLT_LIM, writeVal);
    uint8_t result = read8BitRegister(CHARGE_VOLT_LIM);

    int return_result = (int) result;
    return_result = return_result * 2 + 1504; 
    return return_result;
}

// Vrechg could be set 93.0 - 97.6% of Vfb_reg
// percentage option: 1 - 93.0%,  2 - 94.3%,  3 - 95.2%, 4 - 97.6%
// Return the number of option is set
int setVrechg (int option){
    uint8_t regVal = read8BitRegister(CHARGER_CONT);
    regVal &= 0x3F;
    switch (option)
    {
        case 1:         // 93.0 %
            regVal |= (0b00 << 6);
            break;

        case 2:         // 94.3 %
            regVal |= (0b01 << 6);
            break;

        case 3:         // 95.2 %
            regVal |= (0b10 << 6);
            break;

        case 4:         // 97.6 %
            regVal |= (0b11 << 6); 
            break;

        default:
            return -1;
    }
    writeRegister(CHARGER_CONT, regVal);
    uint8_t readBack = (read8BitRegister(CHARGER_CONT) >> 6) & 0x03;
    return (int) readBack + 1;
}

// Vbat_lowv could be set 30 - 71.4 % of Vfb_reg
// percentage option: 1 - 30%,  2 - 55%,  3 - 66.7%, 4 - 71.4%
// Return: the number of option set
int setVbat_lowv (int option) 
{
    uint8_t regVal = read8BitRegister(PRECHARGE_TERM_CONT);
    regVal &= 0xF9;
    switch (option)
    {
    case 1:
        regVal |= (0b00) << 1;
        break;

    case 2:
        regVal |= (0b01) << 1;
        break;

    case 3:
        regVal |= (0b10) << 1;
        break;

    case 4:
        regVal |= (0b11) << 1;
        break;
    
    default:
        return -1;
    }

    writeRegister(PRECHARGE_TERM_CONT, regVal);
    uint8_t readBack = (read8BitRegister(PRECHARGE_TERM_CONT) >> 1) & 0x03;
    return (int) readBack + 1;
}

// Vbat_short could be determined, not touching right now

// Sets ICHG_REG (charge current limit) in REG0x02/0x03
// Valid range: 400 mA to 20000 mA, in 50 mA steps
// Returns actual current set in mA, or -1 if invalid
int setIchg(int value) {
    if (value < 400 || value > 20000) {
        return -1; // Out of range
    }

    // Convert current to 9-bit value (bits [10:2])
    uint16_t ichg_value = value / 50;

    // Read current 16-bit value
    uint8_t reg_lsb = read8BitRegister(CHARGE_CURR_LIM);  // REG0x02
    uint8_t reg_msb = read8BitRegister(0x03);  // REG0x03
    uint16_t reg = ((uint16_t)reg_msb << 8) | reg_lsb;

    // Clear bits [10:2] (ICHG_REG)
    reg &= ~(0x1FF << 2);

    // Set new ICHG_REG value
    reg |= (ichg_value << 2);

    // Write back 16-bit value
    writeRegister(0x02, reg & 0xFF);         // LSB
    writeRegister(0x03, (reg >> 8) & 0xFF);  // MSB

    // Confirm by reading back
    uint8_t confirm_lsb = read8BitRegister(0x02);
    uint8_t confirm_msb = read8BitRegister(0x03);
    uint16_t confirm = ((uint16_t)confirm_msb << 8) | confirm_lsb;
    uint16_t confirmed_val = (confirm >> 2) & 0x1FF;

    return confirmed_val * 50;  // Return actual current in mA
}

/*
uint16_t read16BitRegister (uint8_t reg) 
{
    uint16_t data = 0;
    Wire.beginTransmission(I2C_BUS_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(I2C_BUS_ADDR, 2);
    uint16_t msb = Wire.read();
    uint16_t lsb = Wire.read();
    data = (msb << 8) | lsb;

    /*f (Wire.available() >= 2) 
    {
        uint8_t msb = Wire.read();
        uint8_t lsb = Wire.read();
        data = (msb << 8) | lsb;
        //Serial.println(data, HEX);
    } else {
        Serial.println("Data read: FAILED");
    }
    
    return data;
}

uint8_t read8BitRegister (uint8_t reg) 
{
    uint8_t data = 0;
    Wire.beginTransmission(I2C_BUS_ADDR);
    Wire.write (reg);
    Wire.endTransmission(false);
    Wire.requestFrom(I2C_BUS_ADDR, 1);

    if (Wire.available())
    {
        data = Wire.read();
    } else 
    {
        Serial.println("Data read: FAILED");
    }
    return data;
}

void writeRegister(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(I2C_BUS_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

*/
