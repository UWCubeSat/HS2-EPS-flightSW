#include "i2c.hpp"

namespace BQ25756 {

// ===========================================================================
// Internal helpers — not exposed in the header
// ===========================================================================

/**
 * Write-then-read (repeated START) transaction.
 *
 * Replaces:
 *   Wire.beginTransmission(addr);
 *   Wire.write(reg);
 *   Wire.endTransmission(false);          // repeated START — bus stays open
 *   Wire.requestFrom(addr, rxLen);
 *   while (Wire.available()) data = Wire.read();
 *
 * Now calls ctx.writeReadFn which routes to:
 *   BatteryMonitor::busWriteRead_out -> Drv::LinuxI2cDriver::writeRead
 *   -> Linux I2C_RDWR ioctl (two i2c_msg structs, no STOP between them)
 */
static bool i2cWriteRead(const I2cContext& ctx, U8 reg, U8* rxBuf, U8 rxLen) {
    U8 txByte = reg;
    Fw::Buffer writeBuf(&txByte, sizeof(txByte));   // write: just the register address
    Fw::Buffer readBuf(rxBuf, rxLen);               // read:  caller-supplied buffer

    Drv::I2cStatus status = ctx.writeReadFn(ctx.devAddr, writeBuf, readBuf);
    return (status == Drv::I2cStatus::I2C_OK);
}

/**
 * Plain write transaction.
 *
 * Replaces:
 *   Wire.beginTransmission(addr);
 *   Wire.write(reg);
 *   Wire.write(val);
 *   Wire.endTransmission();
 *
 * Now calls ctx.writeFn which routes to:
 *   BatteryMonitor::busWrite_out -> Drv::LinuxI2cDriver::write
 *   -> Linux I2C_RDWR ioctl (single write i2c_msg)
 */
static bool i2cWrite(const I2cContext& ctx, U8 reg, U8 val) {
    U8 txBuf[2] = {reg, val};
    Fw::Buffer writeBuf(txBuf, sizeof(txBuf));

    Drv::I2cStatus status = ctx.writeFn(ctx.devAddr, writeBuf);
    return (status == Drv::I2cStatus::I2C_OK);
}

// ===========================================================================
// Public API — drop-in replacements for the original Arduino Wire helpers
// ===========================================================================

// Replaces: uint8_t read8bitRegister(uint8_t reg)
U8 read8bitRegister(const I2cContext& ctx, U8 reg) {
    U8 data = 0x00;
    i2cWriteRead(ctx, reg, &data, 1);
    // Error is logged by the caller (BatteryMonitor) via I2cStatus return
    // from the port — keeping this helper free of any logging dependency.
    return data;
}

// Replaces: uint16_t read16bitRegister(uint8_t reg)
// BQ25756 sends LSB [7:0] first, then MSB [15:8] — same byte order as Wire impl
U16 read16bitRegister(const I2cContext& ctx, U8 reg) {
    U8 buf[2] = {0x00, 0x00};
    if (!i2cWriteRead(ctx, reg, buf, 2)) {
        return 0xFFFF;  // sentinel error value, matches original
    }
    // buf[0] = LSB, buf[1] = MSB
    return static_cast<U16>((static_cast<U16>(buf[1]) << 8) | buf[0]);
}

// Replaces: void writeRegister(uint8_t reg, uint8_t val)
void writeRegister(const I2cContext& ctx, U8 reg, U8 val) {
    i2cWrite(ctx, reg, val);
}

}  // namespace BQ25756
