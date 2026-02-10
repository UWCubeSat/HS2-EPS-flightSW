#ifndef FAULT_STATUS_H
#define FAULT_STATUS_H

#include "BQ25756.h"

class BQ25756::FaultStatus{
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