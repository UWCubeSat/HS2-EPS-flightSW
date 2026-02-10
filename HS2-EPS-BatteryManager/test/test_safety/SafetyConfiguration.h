#ifndef SAFETY_CONFIGURATION_H
#define SAFETY_CONFIGURATION_H

#include "BQ25756_reg.h"
#include "i2c.h"
#include "BQ25756.h"

class BQ25756::SafetyConfig{
    public:
        void disableWatchdogTimerControl();
        bool WatchdogTimerControlDisabled();
        void EnableTSPinFunctionControl();
        void enableMPPT();
        void disableMPPT();
        bool MPPTenabled(); 
        bool TSPinFunctionControlEnabled();
        bool MPPTINTFlagNormal();
        bool TSFlagNormal();
        bool PGFlagNormal(); 
        bool chargeEnableControl();
        void interruptCharging(); 
        void enableCharging();   
};

#endif