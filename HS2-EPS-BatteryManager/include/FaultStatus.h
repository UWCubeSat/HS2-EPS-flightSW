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

    private:
        // Returns true if the fault at the given FAULT_MASK bit position is unmasked
        // (i.e., will generate an INT pulse). bit 7 = InputUV, 6 = InputOV, etc.
        bool isFaultUnmasked(uint8_t bit);
};

#endif