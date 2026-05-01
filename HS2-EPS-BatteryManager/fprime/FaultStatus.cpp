#include "FaultStatus.hpp"
#include "i2c.hpp"

namespace BQ25756 {

FaultStatus::FaultStatus(const char* compName)
    : FaultStatusComponentBase(compName)
{}

// ===========================================================================
// Existing methods — preserved unchanged
// FAULT_MASK: bit = 0 means fault IS active (generates INT pulse)
// ===========================================================================

/**
 * @brief Check if input under-voltage fault generates an INT pulse.
 * Reads FAULT_MASK bit 7.
 * @param[in] ctx  I2C context.
 * @return true if input under-voltage event produces INT pulse, false if masked.
 */
bool FaultStatus::InputUnderVoltageFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 7) & 0x01) == 0;
}

/**
 * @brief Check if input over-voltage fault generates an INT pulse.
 * Reads FAULT_MASK bit 6.
 * @param[in] ctx  I2C context.
 * @return true if input over-voltage event produces INT pulse, false if masked.
 */
bool FaultStatus::InputOverVoltageFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 6) & 0x01) == 0;
}

/**
 * @brief Check if battery over-current fault generates an INT pulse.
 * Reads FAULT_MASK bit 5.
 * @param[in] ctx  I2C context.
 * @return true if battery over-current event produces INT pulse, false if masked.
 */
bool FaultStatus::BatteryOverCurrentFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 5) & 0x01) == 0;
}

/**
 * @brief Check if battery over-voltage fault generates an INT pulse.
 * Reads FAULT_MASK bit 4.
 * @param[in] ctx  I2C context.
 * @return true if battery over-voltage event produces INT pulse, false if masked.
 */
bool FaultStatus::BatteryOverVoltageFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 4) & 0x01) == 0;
}

/**
 * @brief Check if thermal shutdown fault generates an INT pulse.
 * Reads FAULT_MASK bit 3.
 * @param[in] ctx  I2C context.
 * @return true if TSHUT event produces INT pulse, false if masked.
 */
bool FaultStatus::ThermalShutdownFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 3) & 0x01) == 0;
}

/**
 * @brief Check if charge safety timer expiry generates an INT pulse.
 * Reads FAULT_MASK bit 2.
 * @param[in] ctx  I2C context.
 * @return true if timer expiry rising edge produces INT pulse, false if masked.
 */
bool FaultStatus::ChargeSafetyTimerFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 2) & 0x01) == 0;
}

/**
 * @brief Check if DRV_SUP pin fault generates an INT pulse.
 * Reads FAULT_MASK bit 1.
 * @param[in] ctx  I2C context.
 * @return true if DRV_SUP pin fault produces INT pulse, false if masked.
 */
bool FaultStatus::DRV_SUPPinFault(const I2cContext& ctx) {
    return ((read8bitRegister(ctx, FAULT_MASK) >> 1) & 0x01) == 0;
}

// ===========================================================================
// FPrime handlers — NEW, call existing methods above
// ===========================================================================

/**
 * @brief Periodic fault check — called every rate group tick.
 * Calls checkFaults() which calls all existing fault methods.
 */
void FaultStatus::schedIn_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    checkFaults(makeCtx());
}

/**
 * @brief FAULT_CHECK command handler — on-demand fault read.
 * Calls checkFaults() which calls all existing fault methods.
 */
void FaultStatus::FAULT_CHECK_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    checkFaults(makeCtx());
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief Internal helper — calls every existing fault method and logs
 *        a WARNING_HI event for each active fault.
 *
 * Shared by schedIn_handler and FAULT_CHECK_cmdHandler so fault
 * reading logic is written exactly once.
 *
 * @param[in] ctx  I2C context.
 */
void FaultStatus::checkFaults(const I2cContext& ctx) {
    // Publish raw register for telemetry
    U8 mask = read8bitRegister(ctx, FAULT_MASK);
    tlmWrite_FaultMaskRaw(mask);

    bool anyFault = false;

    // Call each existing method — they re-read the register per call,
    // but keep the logic self-contained and testable independently.
    if (InputUnderVoltageFault(ctx))  { log_WARNING_HI_InputUnderVoltageFault();  anyFault = true; }
    if (InputOverVoltageFault(ctx))   { log_WARNING_HI_InputOverVoltageFault();   anyFault = true; }
    if (BatteryOverCurrentFault(ctx)) { log_WARNING_HI_BatteryOverCurrentFault(); anyFault = true; }
    if (BatteryOverVoltageFault(ctx)) { log_WARNING_HI_BatteryOverVoltageFault(); anyFault = true; }
    if (ThermalShutdownFault(ctx))    { log_WARNING_HI_ThermalShutdownFault();    anyFault = true; }
    if (ChargeSafetyTimerFault(ctx))  { log_WARNING_HI_ChargeSafetyTimerFault();  anyFault = true; }
    if (DRV_SUPPinFault(ctx))         { log_WARNING_HI_DRV_SUPPinFault();         anyFault = true; }

    if (!anyFault) {
        log_ACTIVITY_LO_NoFaultsDetected();
    }
}

// ===========================================================================
// I2cContext builder
// ===========================================================================

static FaultStatus* s_fsInstance = nullptr;

static Drv::I2cStatus fsWriteReadTrampoline(U8 addr, Fw::Buffer& w, Fw::Buffer& r) {
    return s_fsInstance->busWriteRead_out(0, addr, w, r);
}
static Drv::I2cStatus fsWriteTrampoline(U8 addr, Fw::Buffer& w) {
    return s_fsInstance->busWrite_out(0, addr, w);
}

I2cContext FaultStatus::makeCtx() {
    s_fsInstance = this;
    return I2cContext{ I2C_BUS_ADDR, fsWriteReadTrampoline, fsWriteTrampoline };
}

}  // namespace BQ25756
