#ifndef SAFETY_CONFIGURATION_H
#define SAFETY_CONFIGURATION_H

#include "BQ25756_reg.h"
#include "i2c.h"


class SafetyConfig: public BQ25756{
    public:
        void disableWatchdogTimerControl();
        bool WatchdogTimerControlDisabled();
        bool MPPTenabled();
        void EnableTSPinFunctionControl();
        bool TSPinFunctionControlEnabled();
        bool MPPTINTFlagNormal();
        bool TSFlagNormal();
        bool PGFlagNormal();
};

#endif


