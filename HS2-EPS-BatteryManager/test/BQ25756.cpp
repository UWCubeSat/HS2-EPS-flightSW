// Outer class for BQ25756 should be included here
#include "BQ25756.h"

void BQ25756::getStatus(){
        Serial.println("BQ25756 is initialized");
}

// Enable ADC Control
// ADC should be enabled before reading ADC value
void BQ25756::ADCControl::enableADC()
{
            uint8_t currVal = read8bitRegister(ADC_CONT);
            uint8_t newVal = currVal | 0x80;
            writeRegister(ADC_CONT, newVal);
}

// Return
//      bool: True if ADC control is enabled
bool BQ25756::ADCControl::ADCEnabled()
{
    return (read8bitRegister(ADC_CONT) >> 7) == 1;
}

// Get ADC conversion rate
// Return
//      bool: True if One-shot conversion, False if Continuous conversion
bool BQ25756::ADCControl::getADCRate()
{
    return (read8bitRegister(ADC_CONT) >> 6);
}

// Enable IAC ADC Control
void BQ25756::ADCControl::enableIAC_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal & 0x7F;
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Enable IBAT ADC Control
void BQ25756::ADCControl::enableIBAT_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal & 0xBF;
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Enable VAC ADC Control
void BQ25756::ADCControl::enableVAC_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal & 0xDF;
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Enable VBAT ADC Control
void BQ25756::ADCControl::enableVBAT_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal & 0xEF;
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Enable TS ADC Control
void BQ25756::ADCControl::enableTS_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal & 0xFB;
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Enable VFB ADC Control
void BQ25756::ADCControl::enableVFB_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal & 0xFD;
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Enable All ADC Channel Control
// This enable ADC Control for IAC, IBAT, VAC, VBAT, TS, VFB
void BQ25756::ADCControl::enableAllADCControl()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = 0x00;
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

