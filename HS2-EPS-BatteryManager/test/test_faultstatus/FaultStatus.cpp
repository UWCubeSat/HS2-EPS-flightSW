#include "FaultStatus.h"
#include "BQ25756.h"
#include "i2c.h"

/**
 * @brief Check if Input Under-Voltage condition is active. 
 * 
 * Reads FAULT_STATUS bit 7.
 * @returns
 * 1 = Device is in Input Under-Voltage protection, 0 = Input voltage is normal
 */
bool BQ25756::FaultStatus::InputUnderVoltageFault() 
{
    return ((read8bitRegister(FAULT_STATUS) >> 7) & 0x01); 
}

/**
 * @brief Check if Input Over-Voltage condition is active.
 * 
 * Reads FAULT_STATUS bit 6.
 * @returns
 * 1 = Device is in Input Over-Voltage protection, 0 = Input voltage is normal
*/
bool BQ25756::FaultStatus::InputOverVoltageFault()
{
    return ((read8bitRegister(FAULT_STATUS) >> 6) & 0x01); 
} 

/**
 * @brief Check if Battery Over-current condition is active.
 * 
 * Reads FAULT_STATUS bit 5.
 * @returns
 * 1 = Device is in Battery Over-current protection, 0 = Battery current is normal
 */
bool BQ25756::FaultStatus::BatteryOverCurrentFault()
{
    return ((read8bitRegister(FAULT_STATUS) >> 5) & 0x01);
}

/**
 *  @brief Check if Battery Over-voltage condition is active.
 * 
 *  Reads FAULT_STATUS bit 4.
 *  @returns
 *  1 = Device is in Battery Over-voltage protection, 0 = Battery voltage is normal
 */
bool BQ25756::FaultStatus::BatteryOverVoltageFault()
{
    return ((read8bitRegister(FAULT_STATUS) >> 4) & 0x01);
}

/**
 *  @brief Check if Thermal shutdown condition is active.
 * 
 *  Reads FAULT_STATUS bit 3
 *  @returns
 *  1 = Device is in thermal shutdown protection, 0 = Thermal condition is normal
 */
bool BQ25756::FaultStatus::ThermalShutdownFault()
{
    return ((read8bitRegister(FAULT_STATUS) >> 3) & 0x01);
}

/**
 *  @brief Check if Charge safety timer status
 * 
 *  Reads FAULT_STATUS bit 2
 *  @returns
 *  1 = Charge safety timer is expired, 0 = Normal
 */
bool BQ25756::FaultStatus::ChargeSafetyTimerFault()
{
    return ((read8bitRegister(FAULT_STATUS) >> 2) & 0x01);
}

/**
 *  @brief Check DRV_SUP pin voltage status
 * 
 *  Reads FAULT_STATUS bit 1
 *  @returns
 *  1 = DRV_SUP pin voltage is out of valid range, 0 = Normal
 */
bool BQ25756::FaultStatus::DRV_SUPPinFault()
{
    return ((read8bitRegister(FAULT_STATUS) >> 1) & 0x01);
}