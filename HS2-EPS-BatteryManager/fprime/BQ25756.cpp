#include "BQ25756.hpp"
#include "BQ25756_ADCControl.hpp"
#include "BatteryMonitor.hpp"
#include "HeatShutup.hpp"
#include "SafetyConfiguration.hpp"
#include "FaultStatus.hpp"
#include "i2c.hpp"

namespace BQ25756 {

// ===========================================================================
// ADCControl — existing methods (preserved, no changes)
// ===========================================================================

/**
 * @brief Check if ADC is enabled.
 * Reads ADC_CONT bit 7.
 * @param[in] ctx  I2C context.
 * @return true if ADC is enabled, false otherwise.
 */
bool ADCControl::isADCEnabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CONT) >> 7) & 0x01;
}

/**
 * @brief Check if ADC conversion rate is one-shot.
 * Reads ADC_CONT bit 6.
 * @param[in] ctx  I2C context.
 * @return true if one-shot, false if continuous.
 */
bool ADCControl::isADCRateOneshot(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CONT) >> 6) & 0x01;
}

/**
 * @brief Check if IBAT ADC channel is disabled.
 * Reads ADC_CHANNEL_CONT bit 6.
 * @param[in] ctx  I2C context.
 */
bool ADCControl::isIBAT_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 6) & 0x01;
}

/**
 * @brief Check if IAC ADC channel is disabled.
 * Reads ADC_CHANNEL_CONT bit 7.
 * @param[in] ctx  I2C context.
 */
bool ADCControl::isIAC_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 7) & 0x01;
}

/**
 * @brief Check if VAC ADC channel is disabled.
 * Reads ADC_CHANNEL_CONT bit 5.
 * @param[in] ctx  I2C context.
 */
bool ADCControl::isVAC_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 5) & 0x01;
}

/**
 * @brief Check if VBAT ADC channel is disabled.
 * Reads ADC_CHANNEL_CONT bit 4.
 * @param[in] ctx  I2C context.
 */
bool ADCControl::isVBAT_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 4) & 0x01;
}

/**
 * @brief Check if TS ADC channel is disabled.
 * Reads ADC_CHANNEL_CONT bit 2.
 * @param[in] ctx  I2C context.
 */
bool ADCControl::isTS_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 2) & 0x01;
}

/**
 * @brief Check if VFB ADC channel is disabled.
 * Reads ADC_CHANNEL_CONT bit 1.
 * @param[in] ctx  I2C context.
 */
bool ADCControl::isVFB_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 1) & 0x01;
}

// Enable functions

/**
 * @brief Set ADC conversion rate to continuous (clears ADC_CONT bit 6).
 * @param[in] ctx  I2C context.
 */
void ADCControl::setADCContinuous(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CONT);
    writeRegister(ctx, ADC_CONT, currVal & ~(1 << 6));
}

/**
 * @brief Enable ADC (sets ADC_CONT bit 7). Call setADCContinuous() first.
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CONT);
    writeRegister(ctx, ADC_CONT, currVal | (1 << 7));
}

/**
 * @brief Enable all ADC channels (writes 0x00 to ADC_CHANNEL_CONT).
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableAllADCControl(const I2cContext& ctx) {
    writeRegister(ctx, ADC_CHANNEL_CONT, 0x00);
}

/**
 * @brief Enable IAC ADC channel (clears ADC_CHANNEL_CONT bit 7).
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableIAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0x7F);
}

/**
 * @brief Enable IBAT ADC channel (clears ADC_CHANNEL_CONT bit 6).
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableIBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xBF);
}

/**
 * @brief Enable VAC ADC channel (clears ADC_CHANNEL_CONT bit 5).
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableVAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xDF);
}

/**
 * @brief Enable VBAT ADC channel (clears ADC_CHANNEL_CONT bit 4).
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableVBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xEF);
}

/**
 * @brief Enable TS ADC channel (clears ADC_CHANNEL_CONT bit 2).
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableTS_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xFB);
}

/**
 * @brief Enable VFB ADC channel (clears ADC_CHANNEL_CONT bit 1).
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableVFB_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xFD);
}

// Disable functions

/**
 * @brief Disable ADC (clears ADC_CONT bit 7).
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CONT);
    writeRegister(ctx, ADC_CONT, currVal & ~(1 << 7));
}

/**
 * @brief Disable IAC ADC channel (sets ADC_CHANNEL_CONT bit 7).
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableIAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 7));
}

/**
 * @brief Disable IBAT ADC channel (sets ADC_CHANNEL_CONT bit 6).
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableIBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 6));
}

/**
 * @brief Disable VAC ADC channel (sets ADC_CHANNEL_CONT bit 5).
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableVAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 5));
}

/**
 * @brief Disable VBAT ADC channel (sets ADC_CHANNEL_CONT bit 4).
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableVBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 4));
}

/**
 * @brief Disable TS ADC channel (sets ADC_CHANNEL_CONT bit 2).
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableTS_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 2));
}

/**
 * @brief Disable VFB ADC channel (sets ADC_CHANNEL_CONT bit 1).
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableVFB_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 1));
}

// ===========================================================================
// ADCControl — FPrime command handlers (NEW — calls existing methods above)
// ===========================================================================

ADCControl::ADCControl(const char* compName)
    : ADCControlComponentBase(compName)
{}

/**
 * @brief ADC_ENABLE command handler.
 * Calls: enableADC()
 */
void ADCControl::ADC_ENABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    enableADC(makeCtx());
    log_ACTIVITY_LO_AdcEnabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief ADC_DISABLE command handler.
 * Calls: disableADC()
 */
void ADCControl::ADC_DISABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    disableADC(makeCtx());
    log_ACTIVITY_LO_AdcDisabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief ADC_SET_RATE command handler.
 *
 * CONTINUOUS → calls setADCContinuous() (clears bit 6)
 * ONESHOT    → sets bit 6 directly (no existing helper, done inline)
 *
 * @param[in] rate  CONTINUOUS or ONESHOT.
 */
void ADCControl::ADC_SET_RATE_cmdHandler(
    FwOpcodeType opCode, U32 cmdSeq, BQ25756_AdcRate rate)
{
    I2cContext ctx = makeCtx();
    switch (rate) {
        case BQ25756_AdcRate::CONTINUOUS:
            setADCContinuous(ctx);   // calls existing method
            break;
        case BQ25756_AdcRate::ONESHOT: {
            U8 currVal = read8bitRegister(ctx, ADC_CONT);
            writeRegister(ctx, ADC_CONT, currVal | (1 << 6));  // set bit 6
            break;
        }
        default:
            cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::VALIDATION_ERROR);
            return;
    }
    log_ACTIVITY_LO_AdcRateSet(rate);
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief ADC_ENABLE_ALL_CHANNELS command handler.
 * Calls: enableAllADCControl()
 */
void ADCControl::ADC_ENABLE_ALL_CHANNELS_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    enableAllADCControl(makeCtx());
    log_ACTIVITY_LO_AllChannelsEnabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief ADC_ENABLE_CHANNEL command handler.
 * Calls: enableIAC_ADC() / enableIBAT_ADC() / enableVAC_ADC() /
 *        enableVBAT_ADC() / enableTS_ADC() / enableVFB_ADC()
 *
 * @param[in] channel  Which ADC channel to enable.
 */
void ADCControl::ADC_ENABLE_CHANNEL_cmdHandler(
    FwOpcodeType opCode, U32 cmdSeq, BQ25756_AdcChannel channel)
{
    I2cContext ctx = makeCtx();
    switch (channel) {
        case BQ25756_AdcChannel::IAC:  enableIAC_ADC(ctx);  break;
        case BQ25756_AdcChannel::IBAT: enableIBAT_ADC(ctx); break;
        case BQ25756_AdcChannel::VAC:  enableVAC_ADC(ctx);  break;
        case BQ25756_AdcChannel::VBAT: enableVBAT_ADC(ctx); break;
        case BQ25756_AdcChannel::TS:   enableTS_ADC(ctx);   break;
        case BQ25756_AdcChannel::VFB:  enableVFB_ADC(ctx);  break;
        default:
            cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::VALIDATION_ERROR);
            return;
    }
    log_ACTIVITY_LO_ChannelEnabled(channel);
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief ADC_DISABLE_CHANNEL command handler.
 * Calls: disableIAC_ADC() / disableIBAT_ADC() / disableVAC_ADC() /
 *        disableVBAT_ADC() / disableTS_ADC() / disableVFB_ADC()
 *
 * @param[in] channel  Which ADC channel to disable.
 */
void ADCControl::ADC_DISABLE_CHANNEL_cmdHandler(
    FwOpcodeType opCode, U32 cmdSeq, BQ25756_AdcChannel channel)
{
    I2cContext ctx = makeCtx();
    switch (channel) {
        case BQ25756_AdcChannel::IAC:  disableIAC_ADC(ctx);  break;
        case BQ25756_AdcChannel::IBAT: disableIBAT_ADC(ctx); break;
        case BQ25756_AdcChannel::VAC:  disableVAC_ADC(ctx);  break;
        case BQ25756_AdcChannel::VBAT: disableVBAT_ADC(ctx); break;
        case BQ25756_AdcChannel::TS:   disableTS_ADC(ctx);   break;
        case BQ25756_AdcChannel::VFB:  disableVFB_ADC(ctx);  break;
        default:
            cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::VALIDATION_ERROR);
            return;
    }
    log_ACTIVITY_LO_ChannelDisabled(channel);
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ===========================================================================
// ADCControl — I2cContext builder
// ===========================================================================

static ADCControl* s_adcInstance = nullptr;

static Drv::I2cStatus adcWriteReadTrampoline(U8 addr, Fw::Buffer& w, Fw::Buffer& r) {
    return s_adcInstance->busWriteRead_out(0, addr, w, r);
}
static Drv::I2cStatus adcWriteTrampoline(U8 addr, Fw::Buffer& w) {
    return s_adcInstance->busWrite_out(0, addr, w);
}

I2cContext ADCControl::makeCtx() {
    s_adcInstance = this;
    return I2cContext{ I2C_BUS_ADDR, adcWriteReadTrampoline, adcWriteTrampoline };
}

// ===========================================================================
// Top-level reset — preserved unchanged
// ===========================================================================

/**
 * @brief Reset all BQ25756 registers to default values.
 *
 * Writes REG_RST bit (POW_PATH_REV_CONT bit 7) to 1.
 * The hardware clears this bit automatically after reset completes.
 *
 * @param[in] ctx  I2C context.
 */
void resetRegister(const I2cContext& ctx) {
    U8 regRstVal = read8bitRegister(ctx, POW_PATH_REV_CONT);
    writeRegister(ctx, POW_PATH_REV_CONT, regRstVal | 0x80);
}

}  // namespace BQ25756
