#include "i2c.hpp"

namespace BQ25756 {

/**
 * @brief Perform a write-then-read (repeated START) I2C transaction.
 *
 * Replaces the following Arduino Wire.h sequence:
 * @code
 *   Wire.beginTransmission(addr);
 *   Wire.write(reg);
 *   Wire.endTransmission(false);       // repeated START — bus stays open
 *   Wire.requestFrom(addr, rxLen);
 *   while (Wire.available()) data = Wire.read();
 * @endcode
 *
 * Routes through the FPrime port chain:
 *   ctx.writeReadFn -> BatteryMonitor::busWriteRead_out
 *                   -> Drv::LinuxI2cDriver::writeRead
 *                   -> Linux I2C_RDWR ioctl (two i2c_msg, no STOP between)
 *
 * @note This function is internal to i2c.cpp and not exposed in i2c.hpp.
 *
 * @param[in]  ctx    I2C context holding the device address and port function pointers.
 * @param[in]  reg    Register address byte to write first.
 * @param[out] rxBuf  Caller-supplied buffer to fill with read bytes.
 * @param[in]  rxLen  Number of bytes to read.
 *
 * @return true   Transaction succeeded (I2C_OK).
 * @return false  Transaction failed; error is logged by the caller.
 */ 
static bool i2cWriteRead(const I2cContext& ctx, U8 reg, U8* rxBuf, U8 rxLen) {
    U8 txByte = reg;
    Fw::Buffer writeBuf(&txByte, sizeof(txByte));   // write: just the register address
    Fw::Buffer readBuf(rxBuf, rxLen);               // read:  caller-supplied buffer

    Drv::I2cStatus status = ctx.writeReadFn(ctx.devAddr, writeBuf, readBuf);
    return (status == Drv::I2cStatus::I2C_OK);
}

/**
 * @brief I2C write transaction.
 *
 * This code replaces the following code using Wire.h:
 * @code
 *   Wire.beginTransmission(addr);
 *   Wire.write(reg);
 *   Wire.write(val);
 *   Wire.endTransmission();
 * @endcode
 * 
 * Now calls ctx.writeFn which routes to:
 *   BatteryMonitor::busWrite_out -> Drv::LinuxI2cDriver::write
 *   -> Linux I2C_RDWR ioctl (single write i2c_msg)
 * @note This function is internal to i2c.cpp and not exposed in i2c.hpp.
 * 
 * @param[in]  ctx    I2C context holding the device address and port function pointers.
 * @param[in]  reg    Register address byte to write.
 * @param[in]  val    Value to write register.
 * 
 * @return true   Transaction succeeded (I2C_OK).
 * @return false  Transaction failed; error is logged by the caller.
 */
static bool i2cWrite(const I2cContext& ctx, U8 reg, U8 val) {
    U8 txBuf[2] = {reg, val};
    Fw::Buffer writeBuf(txBuf, sizeof(txBuf));

    Drv::I2cStatus status = ctx.writeFn(ctx.devAddr, writeBuf);
    return (status == Drv::I2cStatus::I2C_OK);
}

// Public API

/**
 * @brief I2C Read 1 byte from register
 *
 * This code replaces @code uint8_t read8bitRegister(uint8_t reg) @endcode
 * 
 * @param[in]  ctx    I2C context holding the device address and port function pointers.
 * @param[in]  reg    Register address byte to read.
 * 
 * @return  8-bit register value on success.
 * @return  0x00 on I2C error.
 */
U8 read8bitRegister(const I2cContext& ctx, U8 reg) {
    U8 data = 0x00;
    i2cWriteRead(ctx, reg, &data, 1);
    // Error is logged by the caller (BatteryMonitor) via I2cStatus return
    // from the port — keeping this helper free of any logging dependency.
    return data;
}

/**
 * @brief I2C Read 2 byte from register (LSB first)
 *
 * This code replaces @code uint16_t read16bitRegister(uint8_t reg) @endcode
 * BQ25756 sends LSB [7:0] first, then MSB [15:8] — same byte order as Wire implementation
 * 
 * @param[in]  ctx    I2C context holding the device address and port function pointers.
 * @param[in]  reg    Register address byte to read.
 * 
 * @return  16-bit register value on success
 * @return  0xFFFF on I2C error (sentinel value matching original implementation).
 */
U16 read16bitRegister(const I2cContext& ctx, U8 reg) {
    U8 buf[2] = {0x00, 0x00};
    if (!i2cWriteRead(ctx, reg, buf, 2)) {
        return 0xFFFF;  // sentinel error value, matches original
    }
    // buf[0] = LSB, buf[1] = MSB
    return static_cast<U16>((static_cast<U16>(buf[1]) << 8) | buf[0]);
}

// Replaces: void writeRegister(uint8_t reg, uint8_t val)
/**
 * @brief I2C Write register
 *
 * This code replaces @code void writeRegister(uint8_t reg, uint8_t val) @endcode
 * 
 * @param[in]  ctx    I2C context holding the device address and port function pointers.
 * @param[in]  reg    Register address byte to write.
 * @param[in]  val    Value to write into the register.
 */
void writeRegister(const I2cContext& ctx, U8 reg, U8 val) {
    i2cWrite(ctx, reg, val);
}

}  // namespace BQ25756
