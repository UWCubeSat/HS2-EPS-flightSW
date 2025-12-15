// Safety configuration setting
#include "SafetyConfiguration.h"

class SafetyConfig: public BQ25756 {
    // Disable Watchdog timer control
    // Return if watchdog timer control is off
    public: 

        // Disable Watchdog timer control
        void disableWatchdogTimerControl()
        {
            // Set Watchdog control(TIME_CONT[5:4]) 00b
            uint8_t currValue = read8bitRegister(TIME_CONT);
            uint8_t newValue = currValue & ~0x30;
            writeRegister(TIME_CONT, newValue);
        }

        // Return
        //      bool: if Watchdog timer control is disabled
        bool WatchdogTimerControlDisabled()
        {
            return ((read8bitRegister(TIME_CONT) & 0x30) == 0);
        }

        // Enable TS pin function control
        void EnableTSPinFunctionControl()
        {
            uint8_t currValue = read8bitRegister(CHARGE_REGION_CONT);
            // Set bit 0 to 1
            uint8_t newValue = currValue | 0x01;
            writeRegister(CHARGE_REGION_CONT, newValue);
        }
        
        // Return
        //      bool: if TS pin function control is enabled
        bool TSPinFunctionControlEnabled()
        {
            return ((read8bitRegister(CHARGE_REGION_CONT) & 0x01) == 1);
        }
    

        // INT flag

        // Return 
        //      bool: if MPPT INT flag is normal
        bool MPPTINTFlagNormal()
        {
            uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
            return ((currValue & 0x01) == 0);
        }

        // Return
        //      bool: if TS INT Flag is normal
        bool TSFlagNormal()
        {
            uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
            return (((currValue >> 1) & 0x01) == 0);
        }

        // Return
        //      bool: if Input Power Good INT flag is normal
        bool PGFlagNormal()
        {
            uint8_t currValue = read8bitRegister(CHARGER_FLAG_2);
            return (((currValue >> 7) & 0x01) == 0);
        }

        // TODO: Interupt charging immediately
        





}






