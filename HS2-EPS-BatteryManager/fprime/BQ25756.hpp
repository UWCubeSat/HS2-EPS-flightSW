#ifndef BQ25756_HPP
#define BQ25756_HPP

/**
 * @file BQ25756.hpp
 * @brief Top-level namespace and forward declarations for the BQ25756 component suite.
 *
 * Each subsystem is a standalone class in the BQ25756 namespace.
 * There is no owner object — components are instantiated independently
 * and wired together in the FPrime topology.
 */

namespace BQ25756 {
    class ADCControl;
    class BatteryMonitor;
    class HeatShutup;
    class SafetyConfig;
    class FaultStatus;

    /// Reset all registers to default values (POW_PATH_REV_CONT bit 7).
    void resetRegister(const struct I2cContext& ctx);
}

#endif  // BQ25756_HPP
