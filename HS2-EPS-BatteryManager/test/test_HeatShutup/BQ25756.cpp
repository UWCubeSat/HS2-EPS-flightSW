// Outer class for BQ25756 should be included here
#include "BQ25756.h"

void BQ25756::printInitializationStatus(){
        Serial.println("BQ25756 is initialized");
}

// Return
//      bool: True if ADC control is enabled
bool BQ25756::ADCControl::isADCEnabled()
{
    return (read8bitRegister(ADC_CONT) >> 7);
}

// Get ADC conversion rate
// Return
//      bool: True if One-shot conversion, False if Continuous conversion
bool BQ25756::ADCControl::isADCRateOneshot()
{
    return (read8bitRegister(ADC_CONT) >> 6);
}

// Check if IBAT ADC is disabled
// Return
//      bool : True if IBAT ADC control is disabled, False otherwise
bool BQ25756::ADCControl::isIBAT_ADCDisabled()
{
    return ((read8bitRegister(ADC_CHANNEL_CONT) >> 6) & 0x01);
}

// Check if IAC ADC is disabled
// Return
//      bool : True if IAC ADC disabled, False otherwise
bool BQ25756::ADCControl::isIAC_ADCDisabled() 
{
    return ((read8bitRegister(ADC_CHANNEL_CONT) >> 7) & 0x01);
}

// Check if VAC ADC is disabled
// Return
//      bool : True if VAC ADC disabled, False otherwise
bool BQ25756::ADCControl::isVAC_ADCDisabled() 
{
    return ((read8bitRegister(ADC_CHANNEL_CONT) >> 5) & 0x01);
}

// Check if VBAT ADC is disabled
// Return
//      bool : True if VBAT ADC disabled, False otherwise
bool BQ25756::ADCControl::isVBAT_ADCDisabled() 
{
    return ((read8bitRegister(ADC_CHANNEL_CONT) >> 4) & 0x01);
}

// Check if TS ADC is disabled
// Return
//      bool : True if TS ADC disabled, False otherwise
bool BQ25756::ADCControl::isTS_ADCDisabled() 
{
    return ((read8bitRegister(ADC_CHANNEL_CONT) >> 2) & 0x01);
}

// Check if VFB ADC is disabled
// Return
//      bool : True if VFB ADC disabled, False otherwise
bool BQ25756::ADCControl::isVFB_ADCDisabled() 
{
    return ((read8bitRegister(ADC_CHANNEL_CONT) >> 1) & 0x01);
}


//------------------------------ Enable function-------------------------------------------

// Enable ADC Control
// ADC should be enabled before reading ADC value
void BQ25756::ADCControl::enableADC()
{
            uint8_t currVal = read8bitRegister(ADC_CONT);
            uint8_t newVal = currVal | (1 << 7);
            writeRegister(ADC_CONT, newVal);
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


//------------------------------ Disable function-------------------------------------------

// Disable ADC Control
void BQ25756::ADCControl::disableADC()
{
            uint8_t currVal = read8bitRegister(ADC_CONT);
            uint8_t newVal = currVal & ~(1 << 7);
            writeRegister(ADC_CONT, newVal);
}

// Disable IAC ADC Control
void BQ25756::ADCControl::disableIAC_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal | (1 << 7);
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Disable IBAT ADC Control
void BQ25756::ADCControl::disableIBAT_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal | (1 << 6);
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Disable VAC ADC Control
void BQ25756::ADCControl::disableVAC_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal | (1 << 5);
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Disable VBAT ADC Control
void BQ25756::ADCControl::disableVBAT_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal | (1 << 4);
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Disable TS ADC Control
void BQ25756::ADCControl::disableTS_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal | (1 << 2);
    writeRegister(ADC_CHANNEL_CONT, newVal);
}

// Disable VFB ADC Control
void BQ25756::ADCControl::disableVFB_ADC()
{
    uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
    uint8_t newVal = currVal | (1 << 1);
    writeRegister(ADC_CHANNEL_CONT, newVal);
} 