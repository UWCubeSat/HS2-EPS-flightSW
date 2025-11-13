// Safety configuration setting
#include "i2c.h"

bool disableWatchdogTimerControl()
{
    uint8_t currValue = read8bitRegister(TIME_CONT);
    uint8_t newValue = currValue & ~0x30;
    writeRegister(TIME_CONT, newValue);
    return ((read8bitRegister(TIME_CONT) & 0x30) == 0);
}


// INT flag

// Return if MPPT INT flag is normal
bool MPPTINTFlagNormal()
{
    uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
    return ((currValue && 0x01) == 0);
}

// Return if TS INT Flag is normal
bool TSFlagNormal()
{
    uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
    return (((currValue >> 1) && 0x01) == 0);
}

