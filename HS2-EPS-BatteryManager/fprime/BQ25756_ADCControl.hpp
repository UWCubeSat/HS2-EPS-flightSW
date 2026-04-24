#ifndef BQ25756_ADC_CONTROL_HPP
#define BQ25756_ADC_CONTROL_HPP

#include "i2c.hpp"
#include "BQ25756_reg.h"

namespace BQ25756 {

/**
 * @brief ADC enable/disable control for all BQ25756 ADC channels.
 *
 * All methods require an I2cContext built by the calling FPrime component.
 * No Wire.h, no global objects.
 */
class ADCControl {
  public:
    // Status
    bool isADCEnabled        (const I2cContext& ctx);
    bool isADCRateOneshot    (const I2cContext& ctx);
    bool isIBAT_ADCDisabled  (const I2cContext& ctx);
    bool isIAC_ADCDisabled   (const I2cContext& ctx);
    bool isVAC_ADCDisabled   (const I2cContext& ctx);
    bool isVBAT_ADCDisabled  (const I2cContext& ctx);
    bool isTS_ADCDisabled    (const I2cContext& ctx);
    bool isVFB_ADCDisabled   (const I2cContext& ctx);

    // Enable
    void setADCContinuous    (const I2cContext& ctx);
    void enableADC           (const I2cContext& ctx);
    void enableAllADCControl (const I2cContext& ctx);
    void enableIAC_ADC       (const I2cContext& ctx);
    void enableIBAT_ADC      (const I2cContext& ctx);
    void enableVAC_ADC       (const I2cContext& ctx);
    void enableVBAT_ADC      (const I2cContext& ctx);
    void enableTS_ADC        (const I2cContext& ctx);
    void enableVFB_ADC       (const I2cContext& ctx);

    // Disable
    void disableADC          (const I2cContext& ctx);
    void disableIAC_ADC      (const I2cContext& ctx);
    void disableIBAT_ADC     (const I2cContext& ctx);
    void disableVAC_ADC      (const I2cContext& ctx);
    void disableVBAT_ADC     (const I2cContext& ctx);
    void disableTS_ADC       (const I2cContext& ctx);
    void disableVFB_ADC      (const I2cContext& ctx);
};

}  // namespace BQ25756

#endif  // BQ25756_ADC_CONTROL_HPP
