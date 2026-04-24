#ifndef BQ25756_FAULT_STATUS_HPP
#define BQ25756_FAULT_STATUS_HPP

#include "i2c.hpp"
#include "BQ25756_reg.h"

namespace BQ25756 {

/**
 * @brief Fault flag register reader.
 *
 * Each method returns true when the corresponding fault is UNMASKED
 * (i.e. will generate an INT pulse). A cleared bit in FAULT_MASK = active fault reporting.
 */
class FaultStatus {
  public:
    bool InputUnderVoltageFault  (const I2cContext& ctx);
    bool InputOverVoltageFault   (const I2cContext& ctx);
    bool BatteryOverCurrentFault (const I2cContext& ctx);
    bool BatteryOverVoltageFault (const I2cContext& ctx);
    bool ThermalShutdownFault    (const I2cContext& ctx);
    bool ChargeSafetyTimerFault  (const I2cContext& ctx);
    bool DRV_SUPPinFault         (const I2cContext& ctx);
};

}  // namespace BQ25756

#endif  // BQ25756_FAULT_STATUS_HPP
