#include "BQ25756_ard.h"
#include <Wire.h>

void getProperties() {
    uint32_t voltage_ADC = getVoltage_ADC();
    Serial.print("VAC ADC Voltage: ");
    Serial.print((float) voltage_ADC);
    Serial.println(" mV");

    uint32_t Vbat = getVBAT_ADC();
    Serial.print("VBAT ADC Voltage: ");
    Serial.print((float) Vbat);
    Serial.println(" mV");

    uint32_t Vfb = getVfb_ADC();
    Serial.print("Vfb ADC Voltage: ");
    Serial.print((float) Vfb);
    Serial.println(" mV");

    int vrechg = readVrechg();
    Serial.print("Vrechg Voltage: ");
    Serial.print((float) vrechg);
    Serial.println(" mV");

    int vbat_lowv = readVbat_lowv();
    Serial.print("Vbat Low Voltage Threshold: ");
    Serial.print((float) vbat_lowv);
    Serial.println(" mV");

    int ichg = readIchg();
    Serial.print("Charge Current Limit: ");
    Serial.print(ichg);
    Serial.println(" mA");

}

void printVBAT_LOWV() {
  uint8_t data = read8BitRegister(PRECHARGE_TERM_CONT);
  uint8_t bit2_1Value = (data & 0x06) >> 1;
  Serial.print("VBAT_LOW is");
  Serial.println(bit2_1Value);
}

uint32_t getVfb_ADC () 
{
  uint16_t data = read16BitRegister(VAC_ADC);
  uint32_t voltage = data * 2;
  return voltage;
}


uint32_t getVoltage_ADC () 
{
    uint16_t data = read16BitRegister(VAC_ADC);
    uint32_t voltage = data * 2;
    return voltage;
}

uint32_t getVBAT_ADC() 
{
  uint32_t data = (uint32_t) read16BitRegister(VBAT_ADC);
  data = data * 2;
  return data;
}

uint16_t getIBAT_ADC() 
{
  uint16_t data = (uint32_t) read16BitRegister(0x30);
  data = data * 2;
  return data;
}
int16_t convertIbatADC(uint16_t adcValue) {
    // Check if the value is negative (MSB is set)
    if (adcValue & 0x8000) {  // If the 15th bit (MSB) is set
        // Convert from 2's complement to negative value
        return -(65536 - adcValue) * 2;  // Subtract from 65536 and then multiply by 2mA
    } else {
        // Positive value, just multiply by the step size
        return adcValue * 2;  // Multiply by 2mA
    }
}

// ----------------------Read Value functions---------------------------
int readVfb_reg() {
    uint8_t regVal = read8BitRegister(CHARGE_VOLT_LIM);
    int value = regVal * 2 + 1504;
    return value;
}

int readVrechg() {
    uint8_t regVal = read8BitRegister(CHARGER_CONT);
    int option = (regVal >> 6) & 0x03; // Get the two bits related to Vrechg
    int vfb_reg = readVfb_reg(); // Read Vfb_reg to get the base voltage
    float percentage = 0.0;

    switch (option) {
        case 0:
            percentage = 0.93; // 93.0%
            break;
        case 1:
            percentage = 0.943; // 94.3%
            break;
        case 2:
            percentage = 0.952; // 95.2%
            break;
        case 3:
            percentage = 0.976; // 97.6%
            break;
        default:
            return -1; // Invalid option
    }

    int vrechg = vfb_reg * percentage; // Calculate Vrechg based on Vfb_reg and percentage
    return vrechg;
}

int readVbat_lowv() {
    uint8_t regVal = read8BitRegister(PRECHARGE_TERM_CONT);
    int option = (regVal >> 1) & 0x03; // Get the two bits related to Vbat_lowv
    int vfb_reg = readVfb_reg(); // Read Vfb_reg to get the base voltage
    float percentage = 0.0;

    switch (option) {
        case 0:
            percentage = 0.30; // 30%
            break;
        case 1:
            percentage = 0.55; // 55%
            break;
        case 2:
            percentage = 0.667; // 66.7%
            break;
        case 3:
            percentage = 0.714; // 71.4%
            break;
        default:
            return -1; // Invalid option
    }

    int vbat_lowv = vfb_reg * percentage; // Calculate Vbat_lowv based on Vfb_reg and percentage
    return vbat_lowv;
}

int readIchg() {
    uint8_t regLsb = read8BitRegister(0x02);  // REG0x02 (LSB)
    uint8_t regMsb = read8BitRegister(0x03);  // REG0x03 (MSB)
    uint16_t reg = ((uint16_t)regMsb << 8) | regLsb;

    // Extract the charge current value (bits 10:2)
    uint16_t ichg_value = (reg >> 2) & 0x1FF;

    // Convert to current in mA (50mA per bit step)
    int ichg = ichg_value * 50;
    return ichg;
}


uint8_t getCHARGE_STAT()
{
    uint8_t data = read8BitRegister(CHARGER_STATUS_1);
    uint8_t bit2_0 = data & 0x07;
    Serial.print("Charging Status: ");
    switch (bit2_0)
    {
        case 0x00:
            Serial.println("Not Charging");
            break;
        case 0x01:
            Serial.println("Trickle Charge (VFB < VBAT_SHORT)");
            break;
        case 0x02:
            Serial.println("Pre-charge (VBAT_SHORT < VFB < VBAT_LOWV)");
            break;
        case 0x03:
            Serial.println("Fast-charge (CC mode)");
            break;
        case 0x04:
            Serial.println("Taper Charge (CV mode)");
            break;
        case 0x05:
            Serial.println("Reserved");
            break;
        case 0x06:
            Serial.println("Top-off Timer Active Charging");
            break;
        case 0x07:
            Serial.println("Charge Termination Done");
            break;
    }

    return bit2_0;
}



uint16_t read16BitRegister (uint8_t reg) 
{
    uint16_t data = 0;
    Wire.beginTransmission(I2C_BUS_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(I2C_BUS_ADDR, 2);
    if (Wire.available()>= 2) {
        uint16_t msb = Wire.read();
        uint16_t lsb = Wire.read();
        data = (msb << 8) | lsb;
    } else {
        Serial.println("Data read: FAILED");
    }
    

    /*f (Wire.available() >= 2) 
    {
        uint8_t msb = Wire.read();
        uint8_t lsb = Wire.read();
        data = (msb << 8) | lsb;
        //Serial.println(data, HEX);
    } else {
        Serial.println("Data read: FAILED");
    }
    */
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
