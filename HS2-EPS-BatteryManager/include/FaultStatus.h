#ifndef FAULT_STATUS_H
#define FAULT_STATUS_H

#include "BQ25756.h"

class FaultStatus: public BQ25756{
    public:
        bool InputUnderVoltageFault();
        bool InputOverVoltageFault();
        bool BatteryOverCurrentFault();
        bool BatteryOverVoltageFault();
        bool ThermalShutdownFault();
        bool ChargeSafetyTimerFault();
        bool DRV_SUPPinFault();

};

#endif