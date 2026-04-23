#ifndef BQ25756_BATTERY_MONITOR_HPP
#define BQ25756_BATTERY_MONITOR_HPP

#include "BQ25756/BatteryMonitorComponentAc.hpp"  // FPrime auto-generated base
#include "i2c.hpp"                                 // read8bitRegister, read16bitRegister, writeRegister
#include "BQ25756_reg.h"                           // Register addresses, I2C_BUS_ADDR

namespace BQ25756 {

/**
 * @brief BQ25756 battery charger manager component.
 *
 * This component is the MANAGER layer in the Application-Manager-Driver pattern.
 * It uses i2c.hpp helper functions for all register access — no Wire.h anywhere.
 *
 * I2C flow:
 *   BatteryMonitor (calls i2c helpers)
 *     -> i2c.cpp (builds Fw::Buffer, calls ctx.writeReadFn / ctx.writeFn)
 *       -> busWriteRead_out / busWrite_out  (FPrime ports on this component)
 *         -> Drv::LinuxI2cDriver            (wired in topology)
 *           -> Linux I2C_RDWR ioctl         (actual hardware)
 *
 * Topology wiring (.fpp):
 *   batteryMonitor.busWriteRead -> linuxI2cDriver.writeRead
 *   batteryMonitor.busWrite     -> linuxI2cDriver.write
 *
 * Topology init (Topology.cpp):
 *   linuxI2cDriver.open("/dev/i2c-1");
 */
class BatteryMonitor : public BatteryMonitorComponentBase {
  public:
    explicit BatteryMonitor(const char* compName);
    ~BatteryMonitor() = default;

  private:
    // -----------------------------------------------------------------------
    // Port handler overrides (auto-generated pure-virtuals)
    // -----------------------------------------------------------------------

    BQ25756_BatteryMeasurements getMeasurements_handler (NATIVE_INT_TYPE portNum) override;
    BQ25756_ChargingStatus      getChargingStatus_handler(NATIVE_INT_TYPE portNum) override;
    I32 getVac_handler (NATIVE_INT_TYPE portNum) override;
    I32 getVbat_handler(NATIVE_INT_TYPE portNum) override;
    I32 getVfb_handler (NATIVE_INT_TYPE portNum) override;
    I32 getIac_handler (NATIVE_INT_TYPE portNum) override;
    I32 getIbat_handler(NATIVE_INT_TYPE portNum) override;

    // -----------------------------------------------------------------------
    // BQ25756 register logic helpers (no I2C knowledge — only call i2c.hpp)
    // -----------------------------------------------------------------------

    I32 getVfbReg();
    I32 readIchg();
    I32 readVrechg();
    I32 readVbat_lowv();

    // -----------------------------------------------------------------------
    // I2C context builder
    // Returns an I2cContext whose function pointers call this component's
    // busWriteRead_out / busWrite_out ports. Passed to every i2c.hpp call.
    // -----------------------------------------------------------------------

    I2cContext makeCtx();
};

}  // namespace BQ25756

#endif  // BQ25756_BATTERY_MONITOR_HPP
