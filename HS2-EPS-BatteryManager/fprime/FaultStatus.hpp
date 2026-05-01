#ifndef BQ25756_FAULT_STATUS_HPP
#define BQ25756_FAULT_STATUS_HPP

#include "BQ25756/FaultStatusComponentAc.hpp"  // FPrime auto-generated base
#include "i2c.hpp"
#include "BQ25756_reg.h"

namespace BQ25756 {

/**
 * @brief Fault flag register reader.
 *
 * Existing methods (one per fault bit) are preserved unchanged.
 * FPrime schedIn + FAULT_CHECK command are added on top — they call
 * the existing methods via the internal checkFaults() helper.
 *
 * Command handler → existing method mapping:
 *   schedIn_handler / FAULT_CHECK_cmdHandler → checkFaults()
 *     → InputUnderVoltageFault()
 *     → InputOverVoltageFault()
 *     → BatteryOverCurrentFault()
 *     → BatteryOverVoltageFault()
 *     → ThermalShutdownFault()
 *     → ChargeSafetyTimerFault()
 *     → DRV_SUPPinFault()
 */
class FaultStatus : public FaultStatusComponentBase {
  public:
    explicit FaultStatus(const char* compName);
    ~FaultStatus() = default;

    // -----------------------------------------------------------------------
    // Existing methods — all preserved, no changes
    // -----------------------------------------------------------------------
    bool InputUnderVoltageFault  (const I2cContext& ctx);
    bool InputOverVoltageFault   (const I2cContext& ctx);
    bool BatteryOverCurrentFault (const I2cContext& ctx);
    bool BatteryOverVoltageFault (const I2cContext& ctx);
    bool ThermalShutdownFault    (const I2cContext& ctx);
    bool ChargeSafetyTimerFault  (const I2cContext& ctx);
    bool DRV_SUPPinFault         (const I2cContext& ctx);

  private:
    // -----------------------------------------------------------------------
    // FPrime handler overrides (NEW)
    // -----------------------------------------------------------------------
    void schedIn_handler       (NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) override;
    void FAULT_CHECK_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) override;

    // -----------------------------------------------------------------------
    // Internal helper — calls all existing fault methods, logs events (NEW)
    // -----------------------------------------------------------------------
    void checkFaults(const I2cContext& ctx);

    I2cContext makeCtx();
};

}  // namespace BQ25756

#endif  // BQ25756_FAULT_STATUS_HPP
