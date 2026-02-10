// Safety configuration setting
#include "SafetyConfiguration.h"

// Disable Watchdog timer control
void BQ25756::SafetyConfig::disableWatchdogTimerControl()
{
    // Set Watchdog control(TIME_CONT[5:4]) 00b
    uint8_t currValue = read8bitRegister(TIME_CONT);
    uint8_t newValue = currValue & ~0x30;
    writeRegister(TIME_CONT, newValue);
}

// Check if Watchdog timer control 
// Return
//      bool: if Watchdog timer control is disabled
bool BQ25756::SafetyConfig::WatchdogTimerControlDisabled()
{
    return ((read8bitRegister(TIME_CONT) & 0x30) == 0);
}


// Disable MPPT
void BQ25756::SafetyConfig::disableMPPT()
{
    uint8_t currValue = read8bitRegister(MPPT_CONT);
    uint8_t newValue = currValue & ~(0x01);
    writeRegister(MPPT_CONT, newValue);
}

// Enable MPPT
void BQ25756::SafetyConfig::enableMPPT()
{
    uint8_t currValue = read8bitRegister(MPPT_CONT);
    uint8_t newValue = currValue | 0x01;
    writeRegister(MPPT_CONT, newValue);
}

/** 
 * @brief Return if MPPT is enabled
 * 
 * When MPPT is enabled, the ADC is controlled by the device, writes to REG2A are ignored
 * @return Return true if MPPT is enabled, otherwise false
 */
bool BQ25756::SafetyConfig::MPPTenabled() 
{
    return ((read8bitRegister(MPPT_CONT) & 0x01) == 1);
}

// Enable TS pin function control
void BQ25756::SafetyConfig::EnableTSPinFunctionControl()
{
    uint8_t currValue = read8bitRegister(CHARGE_REGION_CONT);
    // Set bit 0 to 1
    uint8_t newValue = currValue | 0x01;
    writeRegister(CHARGE_REGION_CONT, newValue);
}

// Check if TS pin function control is enabled
// Return
//      bool: if TS pin function control is enabled
bool BQ25756::SafetyConfig::TSPinFunctionControlEnabled()
{
    return ((read8bitRegister(CHARGE_REGION_CONT) & 0x01) == 1);
}

// INT flag

// Check if MPPT INT flag is normal
// Return 
//      bool: if MPPT INT flag is normal
bool BQ25756::SafetyConfig::MPPTINTFlagNormal()
{
    uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
    return ((currValue & 0x01) == 0);
}

// Check if TS INT Flag is normal
// Return
//      bool: if TS INT Flag is normal
bool BQ25756::SafetyConfig::TSFlagNormal()
{
    uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
    return (((currValue >> 1) & 0x01) == 0);
}

// Check if POwer Good INT flag is normal
// Return
//      bool: if Input Power Good INT flag is normal
bool BQ25756::SafetyConfig::PGFlagNormal()
{
    uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
    return (((currValue >> 7) & 0x01) == 0);
}

// Check Charge enable status: EN_CHG
// Return
//      bool: True if enabled, otherwise false
bool BQ25756::SafetyConfig::chargeEnableControl()
{
    return (read8bitRegister(CHARGER_CONT) & (0x01));
}

// Stop charging battery immediately
void BQ25756::SafetyConfig::interruptCharging()
{
    uint8_t currValue = read8bitRegister(CHARGER_CONT);
    uint8_t newValue = currValue & ~(0x01);
    writeRegister(CHARGER_CONT, newValue);
}

// Enable charging
void BQ25756::SafetyConfig::enableCharging()
{
    uint8_t currValue = read8bitRegister(CHARGER_CONT);
    uint8_t newValue = currValue | (0x01);
    writeRegister(CHARGER_CONT, newValue);
}


