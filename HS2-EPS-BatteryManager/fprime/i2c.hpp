#ifndef BQ25756_I2C_HPP
#define BQ25756_I2C_HPP

#include <Fw/Buffer/Buffer.hpp>
#include <Drv/I2cDriverPorts/I2cStatusEnumAc.hpp>
#include <Fw/Types/BasicTypes.hpp>

// Forward-declare the port output function types so i2c.hpp has no dependency
// on any specific FPrime component. The caller (BatteryMonitor) passes in
// function pointers that wrap its own busWriteRead_out / busWrite_out calls.
namespace BQ25756 {

/**
 * @brief Function pointer types matching the Drv.I2c port signatures.
 *
 * BatteryMonitor passes lambdas/wrappers for these so i2c.cpp never depends
 * on a specific component class — it stays a plain helper, not a component.
 */
using I2cWriteReadFn = Drv::I2cStatus (*)(U8 addr,
                                           Fw::Buffer& writeBuffer,
                                           Fw::Buffer& readBuffer);

using I2cWriteFn     = Drv::I2cStatus (*)(U8 addr,
                                           Fw::Buffer& writeBuffer);

/**
 * @brief Context struct bundling the two bus functions and the device address.
 *
 * Passed by BQ25756 Components into every i2c helper call so the helpers are
 * completely self-contained and reusable by other components.
 */
struct I2cContext {
    U8             devAddr;      ///< 7-bit I2C device address (I2C_BUS_ADDR = 0x6B)
    I2cWriteReadFn writeReadFn;  ///< Wraps busWriteRead_out
    I2cWriteFn     writeFn;      ///< Wraps busWrite_out
};

// ---------------------------------------------------------------------------
// Drop-in replacements for the original Arduino Wire helpers
// ---------------------------------------------------------------------------

/**
 * @brief Read 1 byte from a BQ25756 register.
 *
 * This code replaces @code uint8_t read8bitRegister(uint8_t reg) @endcode
 * 
 * @param ctx  I2C context (device address + port function pointers)
 * @param reg  Register address to read
 * @return     Register value, or 0x00 on error
 */
U8  read8bitRegister (const I2cContext& ctx, U8 reg);

/**
 * @brief Read 2 bytes (LSB first) from a BQ25756 register.
 *
 * This code replaces @code uint16_t read16bitRegister(uint8_t reg) @endcode
 * BQ25756 sends LSB [7:0] first, then MSB [15:8] — same byte order as Wire implementation
 * 
 * @param ctx  I2C context
 * @param reg  Register address to read
 * @return     16-bit register value, or 0xFFFF on error
 */
U16 read16bitRegister(const I2cContext& ctx, U8 reg);

/**
 * @brief Write 1 byte to a BQ25756 register.
 * 
 * This code replaces @code void writeRegister(uint8_t reg, uint8_t val) @endcode
 * 
 * @param ctx  I2C context
 * @param reg  Register address to write
 * @param val  Value to write
 */
void writeRegister(const I2cContext& ctx, U8 reg, U8 val);

}  // namespace BQ25756

#endif  // BQ25756_I2C_HPP
