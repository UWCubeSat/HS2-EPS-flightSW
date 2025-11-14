#ifndef FAULT_STATUS_HPP
#define FAULT_STATUS_HPP

#include "BQ25756.hpp"
class FaultStatus: public BQ25756{
    public:
        bool InputUnderVoltageFault();
        bool InputOverVoltageFault();
        bool BatteryOverCurrentFault();
        bool BatteryOverVoltageFault();
        bool ThermalShutdownFault();
        bool ChargeSafetyTimerFault();
        bool DRV_SUPPinFault();

}