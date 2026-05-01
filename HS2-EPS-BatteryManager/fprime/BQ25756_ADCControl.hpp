#ifndef BQ25756_ADC_CONTROL_HPP
#define BQ25756_ADC_CONTROL_HPP

#include "BQ25756/ADCControlComponentAc.hpp"  // FPrime auto-generated base
#include "i2c.hpp"
#include "BQ25756_reg.h"

namespace BQ25756 {

/**
 * @brief ADC enable/disable control for all BQ25756 ADC channels.
 *
 * Existing methods (ctx-based free-function wrappers) are preserved unchanged.
 * FPrime command handlers are added on top — they call the existing methods.
 *
 * Command handler → existing method mapping:
 *   ADC_ENABLE_cmdHandler            → enableADC()
 *   ADC_DISABLE_cmdHandler           → disableADC()
 *   ADC_SET_RATE_cmdHandler          → setADCContinuous() or sets bit 6 for oneshot
 *   ADC_ENABLE_ALL_CHANNELS_cmdHandler → enableAllADCControl()
 *   ADC_ENABLE_CHANNEL_cmdHandler    → enableIAC_ADC() / enableIBAT_ADC() / ...
 *   ADC_DISABLE_CHANNEL_cmdHandler   → disableIAC_ADC() / disableIBAT_ADC() / ...
 */
class ADCControl : public ADCControlComponentBase {
  public:
    explicit ADCControl(const char* compName);
    ~ADCControl() = default;

    // -----------------------------------------------------------------------
    // Existing methods — all preserved, no changes
    // -----------------------------------------------------------------------

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

  private:
    // -----------------------------------------------------------------------
    // FPrime command handler overrides (auto-generated pure-virtuals)
    // -----------------------------------------------------------------------

    void ADC_ENABLE_cmdHandler(
        FwOpcodeType opCode, U32 cmdSeq) override;

    void ADC_DISABLE_cmdHandler(
        FwOpcodeType opCode, U32 cmdSeq) override;

    void ADC_SET_RATE_cmdHandler(
        FwOpcodeType opCode, U32 cmdSeq,
        BQ25756_AdcRate rate) override;

    void ADC_ENABLE_ALL_CHANNELS_cmdHandler(
        FwOpcodeType opCode, U32 cmdSeq) override;

    void ADC_ENABLE_CHANNEL_cmdHandler(
        FwOpcodeType opCode, U32 cmdSeq,
        BQ25756_AdcChannel channel) override;

    void ADC_DISABLE_CHANNEL_cmdHandler(
        FwOpcodeType opCode, U32 cmdSeq,
        BQ25756_AdcChannel channel) override;

    // -----------------------------------------------------------------------
    // I2C context builder — bridges command handlers to existing methods
    // -----------------------------------------------------------------------
    I2cContext makeCtx();
};

}  // namespace BQ25756

#endif  // BQ25756_ADC_CONTROL_HPP
