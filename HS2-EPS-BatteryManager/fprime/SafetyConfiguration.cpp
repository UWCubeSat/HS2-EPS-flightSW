#include "SafetyConfiguration.hpp"
#include "i2c.hpp"

namespace BQ25756 {

SafetyConfig::SafetyConfig(const char* compName)
    : SafetyConfigComponentBase(compName)
{}

// ===========================================================================
// Existing methods — preserved unchanged
// ===========================================================================

/**
 * @brief Disable watchdog timer control (clears TIME_CONT bits [5:4]).
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::disableWatchdogTimerControl(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, TIME_CONT);
    writeRegister(ctx, TIME_CONT, currValue & ~0x30);
}

/**
 * @brief Check if watchdog timer control is disabled.
 * Reads TIME_CONT bits [5:4].
 * @param[in] ctx  I2C context.
 * @return true if watchdog is disabled (bits [5:4] == 00b), false otherwise.
 */
bool SafetyConfig::WatchdogTimerControlDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, TIME_CONT) & 0x30) == 0;
}

/**
 * @brief Disable MPPT (clears MPPT_CONT bit 0).
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::disableMPPT(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, MPPT_CONT);
    writeRegister(ctx, MPPT_CONT, currValue & ~0x01);
}

/**
 * @brief Enable MPPT (sets MPPT_CONT bit 0).
 * @note When MPPT is enabled, the ADC is controlled by the device
 *       and writes to REG2A are ignored.
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::enableMPPT(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, MPPT_CONT);
    writeRegister(ctx, MPPT_CONT, currValue | 0x01);
}

/**
 * @brief Check if MPPT is enabled (reads MPPT_CONT bit 0).
 * @param[in] ctx  I2C context.
 * @return true if MPPT is enabled, false otherwise.
 */
bool SafetyConfig::MPPTenabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, MPPT_CONT) & 0x01) == 1;
}

/**
 * @brief Enable TS pin function control (sets CHARGE_REGION_CONT bit 0).
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::EnableTSPinFunctionControl(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, CHARGE_REGION_CONT);
    writeRegister(ctx, CHARGE_REGION_CONT, currValue | 0x01);
}

/**
 * @brief Check if TS pin function control is enabled (reads CHARGE_REGION_CONT bit 0).
 * @param[in] ctx  I2C context.
 * @return true if TS pin function control is enabled, false otherwise.
 */
bool SafetyConfig::TSPinFunctionControlEnabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, CHARGE_REGION_CONT) & 0x01) == 1;
}

/**
 * @brief Check if MPPT INT flag is normal (reads CHARGER_FLAG_2 bit 0).
 * @param[in] ctx  I2C context.
 * @return true if MPPT INT flag is normal, false if event occurred.
 */
bool SafetyConfig::MPPTINTFlagNormal(const I2cContext& ctx) {
    return (read8bitRegister(ctx, CHARGER_FLAG_2) & 0x01) == 0;
}

/**
 * @brief Check if TS INT flag is normal (reads CHARGER_FLAG_2 bit 1).
 * @param[in] ctx  I2C context.
 * @return true if TS INT flag is normal, false if event occurred.
 */
bool SafetyConfig::TSFlagNormal(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, CHARGER_FLAG_2) >> 1) & 0x01) == 0;
}

/**
 * @brief Check if Power Good INT flag is normal (reads CHARGER_FLAG_2 bit 7).
 * @param[in] ctx  I2C context.
 * @return true if PG INT flag is normal, false if event occurred.
 */
bool SafetyConfig::PGFlagNormal(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, CHARGER_FLAG_2) >> 7) & 0x01) == 0;
}

/**
 * @brief Check charge enable status — reads CHARGER_CONT bit 0.
 * @param[in] ctx  I2C context.
 * @return true if charging is enabled, false otherwise.
 */
bool SafetyConfig::chargeEnableControl(const I2cContext& ctx) {
    return (read8bitRegister(ctx, CHARGER_CONT) & 0x01) == 1;
}

/**
 * @brief Stop charging immediately (clears CHARGER_CONT bit 0).
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::interruptCharging(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, CHARGER_CONT);
    writeRegister(ctx, CHARGER_CONT, currValue & ~0x01);
}

/**
 * @brief Enable charging (sets CHARGER_CONT bit 0).
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::enableCharging(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, CHARGER_CONT);
    writeRegister(ctx, CHARGER_CONT, currValue | 0x01);
}

// ===========================================================================
// FPrime command handlers — NEW, call existing methods above
// ===========================================================================

/**
 * @brief SC_ENABLE_MPPT command handler.
 * Calls: enableMPPT()
 */
void SafetyConfig::SC_ENABLE_MPPT_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    enableMPPT(makeCtx());
    log_ACTIVITY_LO_MPPTEnabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief SC_DISABLE_MPPT command handler.
 * Calls: disableMPPT()
 */
void SafetyConfig::SC_DISABLE_MPPT_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    disableMPPT(makeCtx());
    log_ACTIVITY_LO_MPPTDisabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief SC_INTERRUPT_CHARGING command handler.
 * Calls: interruptCharging()
 */
void SafetyConfig::SC_INTERRUPT_CHARGING_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    interruptCharging(makeCtx());
    log_WARNING_LO_ChargingInterrupted();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief SC_ENABLE_CHARGING command handler.
 * Calls: enableCharging()
 */
void SafetyConfig::SC_ENABLE_CHARGING_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    enableCharging(makeCtx());
    log_ACTIVITY_LO_ChargingEnabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief SC_DISABLE_WATCHDOG command handler.
 * Calls: disableWatchdogTimerControl()
 */
void SafetyConfig::SC_DISABLE_WATCHDOG_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    disableWatchdogTimerControl(makeCtx());
    log_ACTIVITY_LO_WatchdogDisabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief SC_ENABLE_TS_PIN command handler.
 * Calls: EnableTSPinFunctionControl()
 */
void SafetyConfig::SC_ENABLE_TS_PIN_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    EnableTSPinFunctionControl(makeCtx());
    log_ACTIVITY_LO_TSPinEnabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ===========================================================================
// I2cContext builder
// ===========================================================================

static SafetyConfig* s_scInstance = nullptr;

static Drv::I2cStatus scWriteReadTrampoline(U8 addr, Fw::Buffer& w, Fw::Buffer& r) {
    return s_scInstance->busWriteRead_out(0, addr, w, r);
}
static Drv::I2cStatus scWriteTrampoline(U8 addr, Fw::Buffer& w) {
    return s_scInstance->busWrite_out(0, addr, w);
}

I2cContext SafetyConfig::makeCtx() {
    s_scInstance = this;
    return I2cContext{ I2C_BUS_ADDR, scWriteReadTrampoline, scWriteTrampoline };
}

}  // namespace BQ25756
