#include "SafetyConfiguration.hpp"
#include "i2c.hpp"

namespace BQ25756 {

/**
 * @brief Disable watchdog timer control.
 *
 * Clears TIME_CONT bits [5:4] to 00b.
 *
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::disableWatchdogTimerControl(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, TIME_CONT);
    writeRegister(ctx, TIME_CONT, currValue & ~0x30);
}

/**
 * @brief Check if watchdog timer control is disabled.
 *
 * Reads TIME_CONT bits [5:4].
 *
 * @param[in] ctx  I2C context.
 * @return true if watchdog is disabled (bits [5:4] == 00b), false otherwise.
 */
bool SafetyConfig::WatchdogTimerControlDisabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, TIME_CONT) & 0x30) == 0;
}

/**
 * @brief Disable MPPT (Maximum Power Point Tracking).
 *
 * Clears MPPT_CONT bit 0.
 *
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::disableMPPT(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, MPPT_CONT);
    writeRegister(ctx, MPPT_CONT, currValue & ~0x01);
}

/**
 * @brief Enable MPPT (Maximum Power Point Tracking).
 *
 * Sets MPPT_CONT bit 0.
 *
 * @note When MPPT is enabled, the ADC is controlled by the device
 *       and writes to REG2A are ignored.
 *
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::enableMPPT(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, MPPT_CONT);
    writeRegister(ctx, MPPT_CONT, currValue | 0x01);
}

/**
 * @brief Check if MPPT is enabled.
 *
 * Reads MPPT_CONT bit 0.
 *
 * @param[in] ctx  I2C context.
 * @return true if MPPT is enabled, false otherwise.
 */
bool SafetyConfig::MPPTenabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, MPPT_CONT) & 0x01) == 1;
}

/**
 * @brief Enable TS pin function control.
 *
 * Sets CHARGE_REGION_CONT bit 0. Applies to both forward
 * charging and reverse discharging modes.
 *
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::EnableTSPinFunctionControl(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, CHARGE_REGION_CONT);
    writeRegister(ctx, CHARGE_REGION_CONT, currValue | 0x01);
}

/**
 * @brief Check if TS pin function control is enabled.
 *
 * Reads CHARGE_REGION_CONT bit 0.
 *
 * @param[in] ctx  I2C context.
 * @return true if TS pin function control is enabled, false otherwise.
 */
bool SafetyConfig::TSPinFunctionControlEnabled(const I2cContext& ctx) {
    return (read8bitRegister(ctx, CHARGE_REGION_CONT) & 0x01) == 1;
}

/**
 * @brief Check if MPPT INT flag is normal (no event).
 *
 * Reads CHARGER_FLAG_2 bit 0. Flag is normal when bit is 0.
 *
 * @param[in] ctx  I2C context.
 * @return true if MPPT INT flag is normal, false if event occurred.
 */
bool SafetyConfig::MPPTINTFlagNormal(const I2cContext& ctx) {
    return (read8bitRegister(ctx, CHARGER_FLAG_2) & 0x01) == 0;
}

/**
 * @brief Check if TS INT flag is normal (no event).
 *
 * Reads CHARGER_FLAG_2 bit 1. Flag is normal when bit is 0.
 *
 * @param[in] ctx  I2C context.
 * @return true if TS INT flag is normal, false if event occurred.
 */
bool SafetyConfig::TSFlagNormal(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, CHARGER_FLAG_2) >> 1) & 0x01) == 0;
}

/**
 * @brief Check if Power Good INT flag is normal (no event).
 *
 * Reads CHARGER_FLAG_2 bit 7. Flag is normal when bit is 0.
 *
 * @param[in] ctx  I2C context.
 * @return true if PG INT flag is normal, false if event occurred.
 */
bool SafetyConfig::PGFlagNormal(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, CHARGER_FLAG_2) >> 7) & 0x01) == 0;
}

/**
 * @brief Check charge enable status (EN_CHG bit).
 *
 * Reads CHARGER_CONT bit 0.
 *
 * @param[in] ctx  I2C context.
 * @return true if charging is enabled, false otherwise.
 */
bool SafetyConfig::chargeEnableControl(const I2cContext& ctx) {
    return (read8bitRegister(ctx, CHARGER_CONT) & 0x01) == 1;
}

/**
 * @brief Stop charging immediately.
 *
 * Clears CHARGER_CONT bit 0 (EN_CHG).
 *
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::interruptCharging(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, CHARGER_CONT);
    writeRegister(ctx, CHARGER_CONT, currValue & ~0x01);
}

/**
 * @brief Enable charging.
 *
 * Sets CHARGER_CONT bit 0 (EN_CHG).
 *
 * @param[in] ctx  I2C context.
 */
void SafetyConfig::enableCharging(const I2cContext& ctx) {
    U8 currValue = read8bitRegister(ctx, CHARGER_CONT);
    writeRegister(ctx, CHARGER_CONT, currValue | 0x01);
}

}  // namespace BQ25756
