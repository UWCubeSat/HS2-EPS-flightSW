// ADC control
// Enable ADC control
// Read ADC values : IAC
//                   IBAT
//                   VAC
//                   VBAT
//                   TS
//                   VFB


#include "BQ25756_ard.h"
#include "ADC_control.h"

#define ADC_CHANNEL_CONTROL_REG 0x2C

// Set ADC channel enabled
void enableADC() {
    uint8_t  reg_value = read8BitRegister(ADC_CHANNEL_CONTROL_REG);
    uint8_t mask = 0x00; 
    // Clear the relevant bits to enable the ADC channels
    reg_value &= mask;  // Perform bitwise AND with the mask

    writeRegister(ADC_CHANNEL_CONTROL_REG, reg_value);
}

// Return IAC_ADC in mA
float readIAC_ADC() {
    uint16_t low_byte = read8BitRegister(0x2D);
    uint16_t high_byte = read8BitRegister(0x2E);
    uint16_t readVal = (high_byte << 8) | low_byte;
    int16_t signedVal = (int16_t)readVal;

    float amp = (float)signedVal * 0.8f;
    return amp;
}

// Return IBAT_ADC in mA
float readIBAT_ADC() {
    uint16_t low_byte = read8BitRegister(0x2F);
    uint16_t high_byte = read8BitRegister(0x30);
    uint16_t readVal = (high_byte << 8) | low_byte;
    int16_t signedVal = (int16_t)readVal;

    float amp = (float)signedVal * 2.0f;
    return amp;
}

// Return VAC_ADC in mV
float readVAC_ADC() {
    uint16_t low_byte = read8BitRegister(0x31);
    uint16_t high_byte = read8BitRegister(0x32);
    uint16_t readVal = (high_byte << 8) | low_byte;

    float volt = (float)readVal * 2.0f;
    return volt;
}

// Return VBAT_ADC in mV
float readVBAT_ADC() {
    uint16_t low_byte = read8BitRegister(0x33);
    uint16_t high_byte = read8BitRegister(0x34);
    uint16_t readVal = (high_byte << 8) | low_byte;

    float volt = (float)readVal * 2.0f;
    return volt;
}

// Return TS_ADC based on REGN value (REGN * percentage)
float readTS_ADC(float REGN){
    uint16_t low_byte = read8BitRegister(0x37);
    uint16_t high_byte = read8BitRegister(0x38);
    uint16_t readVal = (high_byte << 8) | low_byte;

    float percentage = (float) readVal * 0.09765625f;
    // Check which is correct
    //float percentage = (float) readVal / 1023.0f * 99.90234375f;
    return percentage * REGN;
}

// Return VFB_ADC in mV
float readVFB_ADC() {
    uint16_t low_byte = read8BitRegister(0x39);
    uint16_t high_byte = read8BitRegister(0x3A);
    uint16_t readVal = (high_byte << 8) | low_byte;

    float volt = (float)readVal * 1.0f;
    return volt;
}

