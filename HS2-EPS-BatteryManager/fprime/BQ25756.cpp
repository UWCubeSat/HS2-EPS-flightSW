#include "BQ25756.hpp"
#include "BatteryMonitor.hpp"
#include "HeatShutup.hpp"
#include "SafetyConfiguration.hpp"
#include "FaultStatus.hpp"
#include "i2c.hpp"

namespace BQ25756 {

// ===========================================================================
// ADCControl
// ===========================================================================

/**
 * @brief Check if ADC is enabled.
 *
 * Reads ADC_CONT bit 7.
 *
 * @param[in] ctx  I2C context.
 * @return true if ADC is enabled, false otherwise.
 */
bool ADCControl::isADCEnabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CONT) >> 7) & 0x01;
}

/**
 * @brief Check if ADC conversion rate is one-shot.
 *
 * Reads ADC_CONT bit 6.
 *
 * @param[in] ctx  I2C context.
 * @return true if one-shot, false if continuous.
 */
bool ADCControl::isADCRateOneshot(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CONT) >> 6) & 0x01;
}

/**
 * @brief Check if IBAT ADC channel is disabled.
 *
 * Reads ADC_CHANNEL_CONT bit 6.
 *
 * @param[in] ctx  I2C context.
 * @return true if IBAT ADC is disabled, false otherwise.
 */
bool ADCControl::isIBAT_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 6) & 0x01;
}

/**
 * @brief Check if IAC ADC channel is disabled.
 *
 * Reads ADC_CHANNEL_CONT bit 7.
 *
 * @param[in] ctx  I2C context.
 * @return true if IAC ADC is disabled, false otherwise.
 */
bool ADCControl::isIAC_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 7) & 0x01;
}

/**
 * @brief Check if VAC ADC channel is disabled.
 *
 * Reads ADC_CHANNEL_CONT bit 5.
 *
 * @param[in] ctx  I2C context.
 * @return true if VAC ADC is disabled, false otherwise.
 */
bool ADCControl::isVAC_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 5) & 0x01;
}

/**
 * @brief Check if VBAT ADC channel is disabled.
 *
 * Reads ADC_CHANNEL_CONT bit 4.
 *
 * @param[in] ctx  I2C context.
 * @return true if VBAT ADC is disabled, false otherwise.
 */
bool ADCControl::isVBAT_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 4) & 0x01;
}

/**
 * @brief Check if TS ADC channel is disabled.
 *
 * Reads ADC_CHANNEL_CONT bit 2.
 *
 * @param[in] ctx  I2C context.
 * @return true if TS ADC is disabled, false otherwise.
 */
bool ADCControl::isTS_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 2) & 0x01;
}

/**
 * @brief Check if VFB ADC channel is disabled.
 *
 * Reads ADC_CHANNEL_CONT bit 1.
 *
 * @param[in] ctx  I2C context.
 * @return true if VFB ADC is disabled, false otherwise.
 */
bool ADCControl::isVFB_ADCDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, ADC_CHANNEL_CONT) >> 1) & 0x01;
}

// ---------------------------------------------------------------------------
// Enable functions
// ---------------------------------------------------------------------------

/**
 * @brief Set ADC conversion rate to continuous.
 *
 * Clears ADC_CONT bit 6. Must be set before enabling ADC,
 * otherwise ADC_EN is always cleared after one-shot.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::setADCContinuous(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CONT);
    writeRegister(ctx, ADC_CONT, currVal & ~(1 << 6));
}

/**
 * @brief Enable ADC.
 *
 * Sets ADC_CONT bit 7. Call setADCContinuous() first.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CONT);
    writeRegister(ctx, ADC_CONT, currVal | (1 << 7));
}

/**
 * @brief Enable all ADC channels (IAC, IBAT, VAC, VBAT, TS, VFB).
 *
 * Writes 0x00 to ADC_CHANNEL_CONT — clearing all disable bits.
 * Call this before reading any ADC measurement register.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableAllADCControl(const I2cContext& ctx) {
    writeRegister(ctx, ADC_CHANNEL_CONT, 0x00);
}

/**
 * @brief Enable IAC ADC channel.
 *
 * Clears ADC_CHANNEL_CONT bit 7.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableIAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0x7F);
}

/**
 * @brief Enable IBAT ADC channel.
 *
 * Clears ADC_CHANNEL_CONT bit 6.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableIBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xBF);
}

/**
 * @brief Enable VAC ADC channel.
 *
 * Clears ADC_CHANNEL_CONT bit 5.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableVAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xDF);
}

/**
 * @brief Enable VBAT ADC channel.
 *
 * Clears ADC_CHANNEL_CONT bit 4.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableVBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xEF);
}

/**
 * @brief Enable TS ADC channel.
 *
 * Clears ADC_CHANNEL_CONT bit 2.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableTS_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xFB);
}

/**
 * @brief Enable VFB ADC channel.
 *
 * Clears ADC_CHANNEL_CONT bit 1.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::enableVFB_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal & 0xFD);
}

// ---------------------------------------------------------------------------
// Disable functions
// ---------------------------------------------------------------------------

/**
 * @brief Disable ADC.
 *
 * Clears ADC_CONT bit 7.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CONT);
    writeRegister(ctx, ADC_CONT, currVal & ~(1 << 7));
}

/**
 * @brief Disable IAC ADC channel.
 *
 * Sets ADC_CHANNEL_CONT bit 7.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableIAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 7));
}

/**
 * @brief Disable IBAT ADC channel.
 *
 * Sets ADC_CHANNEL_CONT bit 6.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableIBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 6));
}

/**
 * @brief Disable VAC ADC channel.
 *
 * Sets ADC_CHANNEL_CONT bit 5.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableVAC_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 5));
}

/**
 * @brief Disable VBAT ADC channel.
 *
 * Sets ADC_CHANNEL_CONT bit 4.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableVBAT_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 4));
}

/**
 * @brief Disable TS ADC channel.
 *
 * Sets ADC_CHANNEL_CONT bit 2.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableTS_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 2));
}

/**
 * @brief Disable VFB ADC channel.
 *
 * Sets ADC_CHANNEL_CONT bit 1.
 *
 * @param[in] ctx  I2C context.
 */
void ADCControl::disableVFB_ADC(const I2cContext& ctx) {
    U8 currVal = read8bitRegister(ctx, ADC_CHANNEL_CONT);
    writeRegister(ctx, ADC_CHANNEL_CONT, currVal | (1 << 1));
}

// ===========================================================================
// Top-level reset
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
