#ifndef BQ25756_BATTERY_MONITOR_HPP
#define BQ25756_BATTERY_MONITOR_HPP

#include "i2c.hpp"
#include "BQ25756_reg.h"
#include <Fw/Types/BasicTypes.hpp>

namespace BQ25756 {

/**
 * @brief Battery voltage, current, and charging status monitor.
 *
 * All methods require an I2cContext built by the calling FPrime component.
 * No Wire.h, no global BQ25756 object, no bq.adc.enableADCReadingForOneshot().
 */
class BatteryMonitor {
  public:
    /// All battery measurements in one struct (mV or mA)
    struct BatteryMeasurements {
        I32 vac;
        I32 vbat;
        I32 vfb;
        I32 vrechg;
        I32 vbat_lowv;
        I32 ichg;
        I32 iac;
        I32 ibat;
    };

    /// Charging status from CHARGER_STATUS_1 bits [2:0]
    enum class ChargingStatus : U8 {
        NOT_CHARGING            = 0x00,
        TRICKLE_CHARGE          = 0x01,
        PRE_CHARGE              = 0x02,
        FAST_CHARGE             = 0x03,
        TAPER_CHARGE            = 0x04,
        RESERVED                = 0x05,
        TOP_OFF_TIMER_ACTIVE    = 0x06,
        CHARGE_TERMINATION_DONE = 0x07
    };

    const char*          toString          (ChargingStatus status);
    BatteryMeasurements  getMeasurements   (const I2cContext& ctx);
    ChargingStatus       getChargingStatus (const I2cContext& ctx);

  private:
    I32 getVac        (const I2cContext& ctx);
    I32 getVbat       (const I2cContext& ctx);
    I32 getVfb        (const I2cContext& ctx);
    I32 getVfbReg     (const I2cContext& ctx);
    I32 getIac        (const I2cContext& ctx);
    I32 getIbat       (const I2cContext& ctx);
    I32 readVrechg    (const I2cContext& ctx);
    I32 readVbat_lowv (const I2cContext& ctx);
    I32 readIchg      (const I2cContext& ctx);
};

}  // namespace BQ25756

#endif  // BQ25756_BATTERY_MONITOR_HPP
