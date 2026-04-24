// Class for INT Status
#include "FaultStatus.h"
#include "BQ25756.h"
#include "i2c.h"

// FAULT_MASK: if unmasked (bit=0), the fault generates an INT pulse.
//             if masked   (bit=1), the fault is silenced.

// Returns true when the fault at `bit` position is unmasked (will produce INT pulse).
bool BQ25756::FaultStatus::isFaultUnmasked(uint8_t bit)
{
    return !readBit(FAULT_MASK, 1 << bit);
}

bool BQ25756::FaultStatus::InputUnderVoltageFault()  { return isFaultUnmasked(7); }
bool BQ25756::FaultStatus::InputOverVoltageFault()   { return isFaultUnmasked(6); }
bool BQ25756::FaultStatus::BatteryOverCurrentFault() { return isFaultUnmasked(5); }
bool BQ25756::FaultStatus::BatteryOverVoltageFault() { return isFaultUnmasked(4); }
bool BQ25756::FaultStatus::ThermalShutdownFault()    { return isFaultUnmasked(3); }
bool BQ25756::FaultStatus::ChargeSafetyTimerFault()  { return isFaultUnmasked(2); }
bool BQ25756::FaultStatus::DRV_SUPPinFault()         { return isFaultUnmasked(1); }
