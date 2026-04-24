#include "BatteryMonitor.hpp"
#include "i2c.hpp"

namespace BQ25756 {

/**
 * @brief Convert ChargingStatus enum to a human-readable string.
 *
 * @param[in] status  ChargingStatus enum value.
 * @return Constant string describing the charging state.
 */
const char* BatteryMonitor::toString(ChargingStatus status) {
    switch (status) {
        case ChargingStatus::NOT_CHARGING:            return "Not Charging";
        case ChargingStatus::TRICKLE_CHARGE:          return "Trickle Charge";
        case ChargingStatus::PRE_CHARGE:              return "Pre-Charge";
        case ChargingStatus::FAST_CHARGE:             return "Fast Charge";
        case ChargingStatus::TAPER_CHARGE:            return "Taper Charge";
        case ChargingStatus::RESERVED:                return "Reserved";
        case ChargingStatus::TOP_OFF_TIMER_ACTIVE:    return "Top-Off Timer Active";
        case ChargingStatus::CHARGE_TERMINATION_DONE: return "Charge Termination Done";
        default:                                      return "Unknown Status";
    }
}

/**
 * @brief Read all battery measurements into a struct.
 *
 * @param[in] ctx  I2C context.
 * @return BatteryMeasurements struct with all values in mV or mA.
 */
BatteryMonitor::BatteryMeasurements BatteryMonitor::getMeasurements(const I2cContext& ctx) {
    BatteryMeasurements p;
    p.vac       = getVac(ctx);
    p.vbat      = getVbat(ctx);
    p.vfb       = getVfb(ctx);
    p.vrechg    = readVrechg(ctx);
    p.vbat_lowv = readVbat_lowv(ctx);
    p.ichg      = readIchg(ctx);
    p.iac       = getIac(ctx);
    p.ibat      = getIbat(ctx);
    return p;
}

/**
 * @brief Read VFB ADC value.
 *
 * Range: 0-2047 mV, step = 2 mV.
 *
 * @param[in] ctx  I2C context.
 * @return VFB ADC reading in mV.
 */
I32 BatteryMonitor::getVfb(const I2cContext& ctx) {
    return static_cast<I32>(read16bitRegister(ctx, VFB_ADC)) * 2;
}

/**
 * @brief Read VAC ADC value.
 *
 * Range: 0-65534 mV, step = 2 mV.
 *
 * @param[in] ctx  I2C context.
 * @return VAC ADC reading in mV.
 */
I32 BatteryMonitor::getVac(const I2cContext& ctx) {
    return static_cast<I32>(read16bitRegister(ctx, VAC_ADC)) * 2;
}

/**
 * @brief Read VBAT ADC value.
 *
 * Range: 0-65534 mV, step = 2 mV.
 *
 * @param[in] ctx  I2C context.
 * @return VBAT ADC reading in mV.
 */
I32 BatteryMonitor::getVbat(const I2cContext& ctx) {
    return static_cast<I32>(read16bitRegister(ctx, VBAT_ADC)) * 2;
}

/**
 * @brief Read IAC ADC value.
 *
 * Range: -20000 to 20000 mA, step = 0.8 mA (register x 8 / 10).
 *
 * @param[in] ctx  I2C context.
 * @return IAC ADC reading in mA.
 */
I32 BatteryMonitor::getIac(const I2cContext& ctx) {
    I16 data = static_cast<I16>(read16bitRegister(ctx, IAC_ADC));
    return static_cast<I32>(data) * 8 / 10;
}

/**
 * @brief Read IBAT ADC value.
 *
 * Range: -20000 to 20000 mA, step = 2 mA.
 *
 * @param[in] ctx  I2C context.
 * @return IBAT ADC reading in mA.
 */
I32 BatteryMonitor::getIbat(const I2cContext& ctx) {
    I16 data = static_cast<I16>(read16bitRegister(ctx, IBAT_ADC));
    return static_cast<I32>(data) * 2;
}

/**
 * @brief Read FB Voltage Regulation Limit.
 *
 * Range: 1504-1566 mV, step = 2 mV, offset = 1504 mV.
 * Used as the base voltage for Vrechg and Vbat_lowv calculations.
 *
 * @param[in] ctx  I2C context.
 * @return VFB regulation limit in mV.
 */
I32 BatteryMonitor::getVfbReg(const I2cContext& ctx) {
    return static_cast<I32>(read16bitRegister(ctx, CHARGE_VOLT_LIM)) * 2 + 1504;
}

/**
 * @brief Read battery auto-recharge threshold.
 *
 * Calculated as a percentage of VFB_REG, selected by CHARGER_CONT bits [7:6].
 *
 * @param[in] ctx  I2C context.
 * @return Vrechg threshold in mV, or -1 on invalid register option.
 */
I32 BatteryMonitor::readVrechg(const I2cContext& ctx) {
    U8  regVal  = read8bitRegister(ctx, CHARGER_CONT);
    I32 option  = (regVal >> 6) & 0x03;
    I32 vfb_reg = getVfbReg(ctx);

    float pct = 0.0f;
    switch (option) {
        case 0: pct = 0.930f; break;  // 93.0%
        case 1: pct = 0.943f; break;  // 94.3%
        case 2: pct = 0.952f; break;  // 95.2%
        case 3: pct = 0.976f; break;  // 97.6%
        default: return -1;
    }
    return static_cast<I32>(static_cast<float>(vfb_reg) * pct);
}

/**
 * @brief Read pre-charge to fast-charge transition threshold.
 *
 * Calculated as a percentage of VFB_REG, selected by PRECHARGE_TERM_CONT bits [2:1].
 *
 * @param[in] ctx  I2C context.
 * @return Vbat_lowv threshold in mV, or -1 on invalid register option.
 */
I32 BatteryMonitor::readVbat_lowv(const I2cContext& ctx) {
    U8  regVal  = read8bitRegister(ctx, PRECHARGE_TERM_CONT);
    I32 option  = (regVal >> 1) & 0x03;
    I32 vfb_reg = getVfbReg(ctx);

    float pct = 0.0f;
    switch (option) {
        case 0: pct = 0.300f; break;  // 30.0%
        case 1: pct = 0.550f; break;  // 55.0%
        case 2: pct = 0.667f; break;  // 66.7%
        case 3: pct = 0.714f; break;  // 71.4%
        default: return -1;
    }
    return static_cast<I32>(static_cast<float>(vfb_reg) * pct);
}

/**
 * @brief Read fast charge current regulation limit.
 *
 * Range: 400-20000 mA, step = 50 mA.
 * Reads REG0x02 (LSB) and REG0x03 (MSB); charge current is in bits [10:2].
 *
 * @param[in] ctx  I2C context.
 * @return Charge current limit in mA.
 */
I32 BatteryMonitor::readIchg(const I2cContext& ctx) {
    U8  regLsb     = read8bitRegister(ctx, 0x02);
    U8  regMsb     = read8bitRegister(ctx, 0x03);
    U16 reg        = (static_cast<U16>(regMsb) << 8) | regLsb;
    U16 ichg_value = (reg >> 2) & 0x1FF;  // bits [10:2]
    return static_cast<I32>(ichg_value) * 50;
}

/**
 * @brief Read current charging status.
 *
 * Reads CHARGER_STATUS_1 register bits [2:0].
 *
 * @param[in] ctx  I2C context.
 * @return ChargingStatus enum value.
 */
BatteryMonitor::ChargingStatus BatteryMonitor::getChargingStatus(const I2cContext& ctx) {
    U8 data = read8bitRegister(ctx, CHARGER_STATUS_1);
    return static_cast<ChargingStatus>(data & 0x07);
}

}  // namespace BQ25756
