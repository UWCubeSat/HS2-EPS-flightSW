#ifndef BQ25756_SAFETY_CONFIGURATION_HPP
#define BQ25756_SAFETY_CONFIGURATION_HPP

#include "i2c.hpp"
#include "BQ25756_reg.h"

namespace BQ25756 {

/**
 * @brief Charge safety limits, watchdog, MPPT, and EN_CHG control.
 */
class SafetyConfig {
  public:
    void disableWatchdogTimerControl  (const I2cContext& ctx);
    bool WatchdogTimerControlDisabled (const I2cContext& ctx);
    void enableMPPT                   (const I2cContext& ctx);
    void disableMPPT                  (const I2cContext& ctx);
    bool MPPTenabled                  (const I2cContext& ctx);
    void EnableTSPinFunctionControl   (const I2cContext& ctx);
    bool TSPinFunctionControlEnabled  (const I2cContext& ctx);
    bool MPPTINTFlagNormal            (const I2cContext& ctx);
    bool TSFlagNormal                 (const I2cContext& ctx);
    bool PGFlagNormal                 (const I2cContext& ctx);
    bool chargeEnableControl          (const I2cContext& ctx);
    void interruptCharging            (const I2cContext& ctx);
    void enableCharging               (const I2cContext& ctx);
};

}  // namespace BQ25756

#endif  // BQ25756_SAFETY_CONFIGURATION_HPP
