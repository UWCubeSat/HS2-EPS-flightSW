#include "BatteryMonitor.hpp"

namespace BQ25756 {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

BatteryMonitor::BatteryMonitor(const char* compName)
    : BatteryMonitorComponentBase(compName)
{}

// ---------------------------------------------------------------------------
// I2C context builder
//
// Builds the I2cContext that bridges i2c.hpp free functions -> this
// component's FPrime output ports. Called at the top of every handler
// so the context is always fresh and captures 'this' correctly.
//
// The lambdas are converted to plain function pointers via a small
// static trampoline because I2cContext uses raw function pointers
// (no std::function) to stay embedded-safe.
// ---------------------------------------------------------------------------

// Static trampolines store 'this' between makeCtx() and the port call.
// Only one BatteryMonitor instance is expected; if you need multiple,
// promote these to a per-instance table.
static BatteryMonitor* s_instance = nullptr;

static Drv::I2cStatus writeReadTrampoline(U32 addr,
                                           Fw::Buffer& writeBuf,
                                           Fw::Buffer& readBuf) {
    return s_instance->busWriteRead_out(0, addr, writeBuf, readBuf);
}

static Drv::I2cStatus writeTrampoline(U32 addr, Fw::Buffer& writeBuf) {
    return s_instance->busWrite_out(0, addr, writeBuf);
}

I2cContext BatteryMonitor::makeCtx() {
    s_instance = this;
    return I2cContext{
        I2C_BUS_ADDR,       // 7-bit device address from BQ25756_reg.h
        writeReadTrampoline, // -> busWriteRead_out -> Drv::LinuxI2cDriver
        writeTrampoline      // -> busWrite_out     -> Drv::LinuxI2cDriver
    };
}

// ===========================================================================
// Port handlers — direct translation of the original BatteryMonitor methods.
// Only difference: read8bitRegister / read16bitRegister / writeRegister now
// come from i2c.hpp instead of Wire.h.
// ===========================================================================

// Equivalent to: getMeasurements()
BQ25756_BatteryMeasurements BatteryMonitor::getMeasurements_handler(NATIVE_INT_TYPE /*portNum*/) {
    BQ25756_BatteryMeasurements p;

    p.setvac      (getVac_handler(0));
    p.setvbat     (getVbat_handler(0));
    p.setvfb      (getVfb_handler(0));
    p.setvrechg   (readVrechg());
    p.setvbat_lowv(readVbat_lowv());
    p.setichg     (readIchg());
    p.setiac      (getIac_handler(0));
    p.setibat     (getIbat_handler(0));

    tlmWrite_Vac      (p.getvac());
    tlmWrite_Vbat     (p.getvbat());
    tlmWrite_Vfb      (p.getvfb());
    tlmWrite_Iac      (p.getiac());
    tlmWrite_Ibat     (p.getibat());
    tlmWrite_Ichg     (p.getichg());
    tlmWrite_Vrechg   (p.getvrechg());
    tlmWrite_VbatLowv (p.getvbat_lowv());

    return p;
}

// Equivalent to: getChargingStatus()
BQ25756_ChargingStatus BatteryMonitor::getChargingStatus_handler(NATIVE_INT_TYPE /*portNum*/) {
    I2cContext ctx = makeCtx();
    U8 data   = read8bitRegister(ctx, CHARGER_STATUS_1);
    U8 bit2_0 = data & 0x07;
    return static_cast<BQ25756_ChargingStatus::t>(bit2_0);
}

// Equivalent to: getVfb() — VFB ADC, Range: 0–2047 mV, step = 2 mV
I32 BatteryMonitor::getVfb_handler(NATIVE_INT_TYPE /*portNum*/) {
    I2cContext ctx = makeCtx();
    U16 data = read16bitRegister(ctx, VFB_ADC);
    return static_cast<I32>(data) * 2;
}

// Equivalent to: getVac() — VAC ADC, Range: 0–65534 mV, step = 2 mV
I32 BatteryMonitor::getVac_handler(NATIVE_INT_TYPE /*portNum*/) {
    I2cContext ctx = makeCtx();
    U16 data = read16bitRegister(ctx, VAC_ADC);
    return static_cast<I32>(data) * 2;
}

// Equivalent to: getVbat() — VBAT ADC, Range: 0–65534 mV, step = 2 mV
I32 BatteryMonitor::getVbat_handler(NATIVE_INT_TYPE /*portNum*/) {
    I2cContext ctx = makeCtx();
    U16 data = read16bitRegister(ctx, VBAT_ADC);
    return static_cast<I32>(data) * 2;
}

// Equivalent to: getIac() — IAC ADC, Range: -20000–20000 mA, step = 0.8 mA
I32 BatteryMonitor::getIac_handler(NATIVE_INT_TYPE /*portNum*/) {
    I2cContext ctx = makeCtx();
    I16 data = static_cast<I16>(read16bitRegister(ctx, IAC_ADC));
    return static_cast<I32>(data) * 8 / 10;
}

// Equivalent to: getIbat() — IBAT ADC, Range: -20000–20000 mA, step = 2 mA
I32 BatteryMonitor::getIbat_handler(NATIVE_INT_TYPE /*portNum*/) {
    I2cContext ctx = makeCtx();
    I16 data = static_cast<I16>(read16bitRegister(ctx, IBAT_ADC));
    return static_cast<I32>(data) * 2;
}

// ===========================================================================
// Private BQ25756 register helpers — no I2C knowledge, only call i2c.hpp
// ===========================================================================

// Equivalent to: getVfbReg()
// FB Voltage Regulation Limit — Range: 1504–1566 mV, step = 2 mV, offset = 1504 mV
I32 BatteryMonitor::getVfbReg() {
    I2cContext ctx = makeCtx();
    U16 data = read16bitRegister(ctx, CHARGE_VOLT_LIM);
    return static_cast<I32>(data) * 2 + 1504;
}

// Equivalent to: readVrechg()
// Auto-recharge threshold as % of VFB_REG, from CHARGER_CONT bits [7:6]
I32 BatteryMonitor::readVrechg() {
    I2cContext ctx    = makeCtx();
    U8  regVal  = read8bitRegister(ctx, CHARGER_CONT);
    I32 option  = (regVal >> 6) & 0x03;
    I32 vfb_reg = getVfbReg();

    float pct = 0.0f;
    switch (option) {
        case 0: pct = 0.930f; break;  // 93.0%
        case 1: pct = 0.943f; break;  // 94.3%
        case 2: pct = 0.952f; break;  // 95.2%
        case 3: pct = 0.976f; break;  // 97.6%
        default:
            this->log_WARNING_LO_InvalidRegisterOption(CHARGER_CONT, option);
            return -1;
    }
    return static_cast<I32>(static_cast<float>(vfb_reg) * pct);
}

// Equivalent to: readVbat_lowv()
// Pre-charge to fast-charge threshold as % of VFB_REG, from PRECHARGE_TERM_CONT bits [2:1]
I32 BatteryMonitor::readVbat_lowv() {
    I2cContext ctx    = makeCtx();
    U8  regVal  = read8bitRegister(ctx, PRECHARGE_TERM_CONT);
    I32 option  = (regVal >> 1) & 0x03;
    I32 vfb_reg = getVfbReg();

    float pct = 0.0f;
    switch (option) {
        case 0: pct = 0.300f; break;  // 30.0%
        case 1: pct = 0.550f; break;  // 55.0%
        case 2: pct = 0.667f; break;  // 66.7%
        case 3: pct = 0.714f; break;  // 71.4%
        default:
            this->log_WARNING_LO_InvalidRegisterOption(PRECHARGE_TERM_CONT, option);
            return -1;
    }
    return static_cast<I32>(static_cast<float>(vfb_reg) * pct);
}

// Equivalent to: readIchg()
// Fast charge current limit — Range: 400–20000 mA, step = 50 mA
// REG0x02 = LSB [7:0], REG0x03 = MSB [15:8], charge current in bits [10:2]
I32 BatteryMonitor::readIchg() {
    I2cContext ctx = makeCtx();
    U8  regLsb = read8bitRegister(ctx, 0x02);
    U8  regMsb = read8bitRegister(ctx, 0x03);
    U16 reg    = (static_cast<U16>(regMsb) << 8) | regLsb;

    U16 ichg_value = (reg >> 2) & 0x1FF;  // bits [10:2]
    return static_cast<I32>(ichg_value) * 50;
}

}  // namespace BQ25756
