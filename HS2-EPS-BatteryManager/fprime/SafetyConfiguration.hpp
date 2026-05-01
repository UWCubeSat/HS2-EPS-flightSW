#ifndef BQ25756_SAFETY_CONFIGURATION_HPP
#define BQ25756_SAFETY_CONFIGURATION_HPP

#include "BQ25756/SafetyConfigComponentAc.hpp"  // FPrime auto-generated base
#include "i2c.hpp"
#include "BQ25756_reg.h"

namespace BQ25756 {

/**
 * @brief Charge safety limits, watchdog, MPPT, and EN_CHG control.
 *
 * Existing methods are preserved unchanged.
 * FPrime command handlers are added on top — they call the existing methods.
 *
 * Command handler → existing method mapping:
 *   SC_ENABLE_MPPT_cmdHandler        → enableMPPT()
 *   SC_DISABLE_MPPT_cmdHandler       → disableMPPT()
 *   SC_INTERRUPT_CHARGING_cmdHandler → interruptCharging()
 *   SC_ENABLE_CHARGING_cmdHandler    → enableCharging()
 *   SC_DISABLE_WATCHDOG_cmdHandler   → disableWatchdogTimerControl()
 *   SC_ENABLE_TS_PIN_cmdHandler      → EnableTSPinFunctionControl()
 */
class SafetyConfig : public SafetyConfigComponentBase {
  public:
    explicit SafetyConfig(const char* compName);
    ~SafetyConfig() = default;

    // -----------------------------------------------------------------------
    // Existing methods — all preserved, no changes
    // -----------------------------------------------------------------------
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

  private:
    // -----------------------------------------------------------------------
    // FPrime command handler overrides (NEW)
    // -----------------------------------------------------------------------
    void SC_ENABLE_MPPT_cmdHandler        (FwOpcodeType opCode, U32 cmdSeq) override;
    void SC_DISABLE_MPPT_cmdHandler       (FwOpcodeType opCode, U32 cmdSeq) override;
    void SC_INTERRUPT_CHARGING_cmdHandler (FwOpcodeType opCode, U32 cmdSeq) override;
    void SC_ENABLE_CHARGING_cmdHandler    (FwOpcodeType opCode, U32 cmdSeq) override;
    void SC_DISABLE_WATCHDOG_cmdHandler   (FwOpcodeType opCode, U32 cmdSeq) override;
    void SC_ENABLE_TS_PIN_cmdHandler      (FwOpcodeType opCode, U32 cmdSeq) override;

    I2cContext makeCtx();
};

}  // namespace BQ25756

#endif  // BQ25756_SAFETY_CONFIGURATION_HPP
