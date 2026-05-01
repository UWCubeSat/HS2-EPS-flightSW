#include "HeatShutup.hpp"
#include "i2c.hpp"

namespace BQ25756 {

HeatShutup::HeatShutup(const char* compName)
    : HeatShutupComponentBase(compName),
      m_lastZone(TS_INVALID)
{}

// ===========================================================================
// Existing methods — preserved unchanged
// ===========================================================================

/**
 * @brief Read thermal shutdown status from CHARGER_STATUS_2 bits [6:4].
 * @note The BQ25756 skips T4, so case 4 is returned as TS_HOT (T5).
 * @param[in] ctx  I2C context.
 * @return TS_LVL enum value representing current thermal zone.
 */
HeatShutup::TS_LVL HeatShutup::readTS_STAT(const I2cContext& ctx) {
    U8 readVal = (read8bitRegister(ctx, CHARGER_STATUS_2) & 0x70) >> 4;
    switch (readVal) {
        case 0: return TS_NORMAL;
        case 1: return TS_WARM;
        case 2: return TS_COOL;
        case 3: return TS_COLD;
        case 4: return TS_HOT;
        default: return TS_INVALID;
    }
}

/**
 * @brief Read TS pin voltage as a percentage of REGN.
 * Reads TS_ADC bits [9:0], converts to percentage: (value / 1024) x 100.
 * @param[in] ctx  I2C context.
 * @return TS voltage as percentage of REGN (0.0-100.0).
 */
float HeatShutup::readTSVoltagePercent(const I2cContext& ctx) {
    U16 tsADC = read16bitRegister(ctx, TS_ADC) & 0x03FF;
    return (tsADC / 1024.0f) * 100.0f;
}

/**
 * @brief Enable JEITA charging profile (sets CHARGE_REGION_CONT bit 1).
 * @param[in] ctx  I2C context.
 */
void HeatShutup::JEITA_enable(const I2cContext& ctx) {
    U8 readVal = read8bitRegister(ctx, CHARGE_REGION_CONT);
    writeRegister(ctx, CHARGE_REGION_CONT, readVal | 0x02);
}

/**
 * @brief Disable JEITA charging profile (clears CHARGE_REGION_CONT bit 1).
 * @param[in] ctx  I2C context.
 */
void HeatShutup::JEITA_disable(const I2cContext& ctx) {
    U8 readVal = read8bitRegister(ctx, CHARGE_REGION_CONT);
    writeRegister(ctx, CHARGE_REGION_CONT, readVal & ~0x02);
}

/**
 * @brief Enable TS pin function control (sets CHARGE_REGION_CONT bit 0).
 * @param[in] ctx  I2C context.
 */
void HeatShutup::TS_enable(const I2cContext& ctx) {
    U8 readVal = read8bitRegister(ctx, CHARGE_REGION_CONT);
    writeRegister(ctx, CHARGE_REGION_CONT, readVal | 0x01);
}

/**
 * @brief Disable TS pin function control (clears CHARGE_REGION_CONT bit 0).
 * @param[in] ctx  I2C context.
 */
void HeatShutup::TS_disable(const I2cContext& ctx) {
    U8 readVal = read8bitRegister(ctx, CHARGE_REGION_CONT);
    writeRegister(ctx, CHARGE_REGION_CONT, readVal & ~0x01);
}

/**
 * @brief Configure T5 zone charging threshold (CHARGE_THRESH_CONT bits [7:6]).
 * @note Not exposed as a command — call directly from Topology.cpp at startup.
 * @param[in] ctx        I2C context.
 * @param[in] userInput  Desired T5 threshold percentage.
 */
void HeatShutup::configure_TS_T5_Charging_Threshold(const I2cContext& ctx, TS_T5_prcnt userInput) {
    U8 writeVal = read8bitRegister(ctx, CHARGE_THRESH_CONT);
    switch (userInput) {
        case T5_41p2:   writeVal  =  writeVal & 0x3F;         break;
        case T5_37p7:   writeVal  = (writeVal & 0x3F) | 0x40; break;
        case T5_34p375: writeVal  = (writeVal & 0x3F) | 0x80; break;
        case T5_31p25:  writeVal  = (writeVal & 0x3F) | 0xC0; break;
    }
    writeRegister(ctx, CHARGE_THRESH_CONT, writeVal);
}

/**
 * @brief Configure T3 zone charging threshold (CHARGE_THRESH_CONT bits [5:4]).
 * @note Not exposed as a command — call directly from Topology.cpp at startup.
 * @param[in] ctx        I2C context.
 * @param[in] userInput  Desired T3 threshold percentage.
 */
void HeatShutup::configure_TS_T3_Charging_Threshold(const I2cContext& ctx, TS_T3_prcnt userInput) {
    U8 writeVal = read8bitRegister(ctx, CHARGE_THRESH_CONT);
    switch (userInput) {
        case T3_48p4: writeVal  =  writeVal & 0xCF;         break;
        case T3_44p8: writeVal  = (writeVal & 0xCF) | 0x10; break;
        case T3_41p2: writeVal  = (writeVal & 0xCF) | 0x20; break;
        case T3_37p7: writeVal  = (writeVal & 0xCF) | 0x30; break;
    }
    writeRegister(ctx, CHARGE_THRESH_CONT, writeVal);
}

/**
 * @brief Configure T2 zone charging threshold (CHARGE_THRESH_CONT bits [3:2]).
 * @note Not exposed as a command — call directly from Topology.cpp at startup.
 * @param[in] ctx        I2C context.
 * @param[in] userInput  Desired T2 threshold percentage.
 */
void HeatShutup::configure_TS_T2_Charging_Threshold(const I2cContext& ctx, TS_T2_prcnt userInput) {
    U8 writeVal = read8bitRegister(ctx, CHARGE_THRESH_CONT);
    switch (userInput) {
        case T2_71p1: writeVal  =  writeVal & 0xF3;         break;
        case T2_68p4: writeVal  = (writeVal & 0xF3) | 0x04; break;
        case T2_65p5: writeVal  = (writeVal & 0xF3) | 0x08; break;
        case T2_62p4: writeVal  = (writeVal & 0xF3) | 0x0C; break;
    }
    writeRegister(ctx, CHARGE_THRESH_CONT, writeVal);
}

/**
 * @brief Configure T1 zone charging threshold (CHARGE_THRESH_CONT bits [1:0]).
 * @note Not exposed as a command — call directly from Topology.cpp at startup.
 * @param[in] ctx        I2C context.
 * @param[in] userInput  Desired T1 threshold percentage.
 */
void HeatShutup::configure_TS_T1_Charging_Threshold(const I2cContext& ctx, TS_T1_prcnt userInput) {
    U8 writeVal = read8bitRegister(ctx, CHARGE_THRESH_CONT);
    switch (userInput) {
        case T1_77p15: writeVal  =  writeVal & 0xFC;         break;
        case T1_75p32: writeVal  = (writeVal & 0xFC) | 0x01; break;
        case T1_73p25: writeVal  = (writeVal & 0xFC) | 0x02; break;
        case T1_71p1:  writeVal  = (writeVal & 0xFC) | 0x03; break;
    }
    writeRegister(ctx, CHARGE_THRESH_CONT, writeVal);
}

/**
 * @brief Check if JEITA profile is disabled (reads CHARGE_REGION_CONT bit 1).
 * @param[in] ctx  I2C context.
 * @return true if JEITA is disabled, false if enabled.
 */
bool HeatShutup::isJEITAdisabled(const I2cContext& ctx) {
    return !((read8bitRegister(ctx, CHARGE_REGION_CONT) >> 1) & 0x01);
}

/**
 * @brief Check if TS pin function is disabled (reads CHARGE_REGION_CONT bit 0).
 * @param[in] ctx  I2C context.
 * @return true if TS is disabled, false if enabled.
 */
bool HeatShutup::isTSdisabled(const I2cContext& ctx) {
    return !((read8bitRegister(ctx, CHARGE_REGION_CONT) >> 0) & 0x01);
}

/**
 * @brief Reset all TS threshold levels to factory defaults.
 * @note Not exposed as a command — call directly from Topology.cpp at startup.
 * Default values: T5=34.375%, T3=44.8%, T2=68.4%, T1=73.25%.
 * @param[in] ctx  I2C context.
 */
void HeatShutup::reset_TS_lvl(const I2cContext& ctx) {
    configure_TS_T5_Charging_Threshold(ctx, T5_34p375);
    configure_TS_T3_Charging_Threshold(ctx, T3_44p8);
    configure_TS_T2_Charging_Threshold(ctx, T2_68p4);
    configure_TS_T1_Charging_Threshold(ctx, T1_73p25);
}

// ===========================================================================
// schedIn handler — periodic thermal telemetry
// ===========================================================================

/**
 * @brief Periodic thermal telemetry update — called every rate group tick.
 *
 * Calls existing methods:
 *   readTS_STAT()          → ThermalZone telemetry + ThermalZoneChanged event on change
 *   readTSVoltagePercent() → TSVoltagePercent telemetry
 *   isJEITAdisabled()      → JEITAEnabled telemetry
 *   isTSdisabled()         → TSEnabled telemetry
 */
void HeatShutup::schedIn_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    I2cContext ctx = makeCtx();

    TS_LVL zone = readTS_STAT(ctx);
    if (zone != m_lastZone) {
        log_ACTIVITY_LO_ThermalZoneChanged(static_cast<BQ25756_TS_LVL>(zone));
        m_lastZone = zone;
    }
    tlmWrite_ThermalZone(static_cast<BQ25756_TS_LVL>(zone));
    tlmWrite_TSVoltagePercent(readTSVoltagePercent(ctx));
    tlmWrite_JEITAEnabled(!isJEITAdisabled(ctx));
    tlmWrite_TSEnabled(!isTSdisabled(ctx));
}

// ===========================================================================
// Command handlers — 4 only
// ===========================================================================

/**
 * @brief HS_JEITA_ENABLE command handler.
 * Calls: JEITA_enable()
 */
void HeatShutup::HS_JEITA_ENABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    JEITA_enable(makeCtx());
    log_ACTIVITY_LO_JEITAEnabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief HS_JEITA_DISABLE command handler.
 * Calls: JEITA_disable()
 */
void HeatShutup::HS_JEITA_DISABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    JEITA_disable(makeCtx());
    log_ACTIVITY_LO_JEITADisabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief HS_TS_ENABLE command handler.
 * Calls: TS_enable()
 */
void HeatShutup::HS_TS_ENABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    TS_enable(makeCtx());
    log_ACTIVITY_LO_TSEnabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

/**
 * @brief HS_TS_DISABLE command handler.
 * Calls: TS_disable()
 */
void HeatShutup::HS_TS_DISABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    TS_disable(makeCtx());
    log_ACTIVITY_LO_TSDisabled();
    cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ===========================================================================
// I2cContext builder
// ===========================================================================

static HeatShutup* s_hsInstance = nullptr;

static Drv::I2cStatus hsWriteReadTrampoline(U8 addr, Fw::Buffer& w, Fw::Buffer& r) {
    return s_hsInstance->busWriteRead_out(0, addr, w, r);
}
static Drv::I2cStatus hsWriteTrampoline(U8 addr, Fw::Buffer& w) {
    return s_hsInstance->busWrite_out(0, addr, w);
}

I2cContext HeatShutup::makeCtx() {
    s_hsInstance = this;
    return I2cContext{ I2C_BUS_ADDR, hsWriteReadTrampoline, hsWriteTrampoline };
}

}  // namespace BQ25756
