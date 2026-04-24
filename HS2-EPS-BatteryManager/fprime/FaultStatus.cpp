#include "FaultStatus.hpp"
#include "i2c.hpp"

namespace BQ25756 {

// FAULT_MASK register: a cleared bit (0) means the fault WILL generate an INT pulse.
// Each function returns true when the fault is active (bit == 0 = unmasked).

/**
 * @brief Check if input under-voltage fault generates an INT pulse.
 *
 * Reads FAULT_MASK bit 7.
 *
 * @param[in] ctx  I2C context.
 * @return true if input under-voltage event produces INT pulse, false if masked.
 */
bool FaultStatus::InputUnderVoltageFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 7) & 0x01) == 0;
}

/**
 * @brief Check if input over-voltage fault generates an INT pulse.
 *
 * Reads FAULT_MASK bit 6.
 *
 * @param[in] ctx  I2C context.
 * @return true if input over-voltage event produces INT pulse, false if masked.
 */
bool FaultStatus::InputOverVoltageFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 6) & 0x01) == 0;
}

/**
 * @brief Check if battery over-current fault generates an INT pulse.
 *
 * Reads FAULT_MASK bit 5.
 *
 * @param[in] ctx  I2C context.
 * @return true if battery over-current event produces INT pulse, false if masked.
 */
bool FaultStatus::BatteryOverCurrentFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 5) & 0x01) == 0;
}

/**
 * @brief Check if battery over-voltage fault generates an INT pulse.
 *
 * Reads FAULT_MASK bit 4.
 *
 * @param[in] ctx  I2C context.
 * @return true if battery over-voltage event produces INT pulse, false if masked.
 */
bool FaultStatus::BatteryOverVoltageFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 4) & 0x01) == 0;
}

/**
 * @brief Check if thermal shutdown fault generates an INT pulse.
 *
 * Reads FAULT_MASK bit 3.
 *
 * @param[in] ctx  I2C context.
 * @return true if TSHUT event produces INT pulse, false if masked.
 */
bool FaultStatus::ThermalShutdownFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 3) & 0x01) == 0;
}

/**
 * @brief Check if charge safety timer expiry generates an INT pulse.
 *
 * Reads FAULT_MASK bit 2.
 *
 * @param[in] ctx  I2C context.
 * @return true if timer expiry rising edge produces INT pulse, false if masked.
 */
bool FaultStatus::ChargeSafetyTimerFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 2) & 0x01) == 0;
}

/**
 * @brief Check if DRV_SUP pin fault generates an INT pulse.
 *
 * Reads FAULT_MASK bit 1.
 *
 * @param[in] ctx  I2C context.
 * @return true if DRV_SUP pin fault produces INT pulse, false if masked.
 */
bool FaultStatus::DRV_SUPPinFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 1) & 0x01) == 0;
}

}  // namespace BQ25756
