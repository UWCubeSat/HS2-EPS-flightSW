// Class for INT Status
#include "FaultStatus.h"
#include "BQ25756.h"
#include "i2c.h"

class FaultStatus: public BQ25756{
    public:

        // Check if Input under-voltage event produces INT pulse
        // Return
        //      bool: True if Input under-voltage event produces INT pulse

        /**
         * @brief Check Input Under-Voltage fault status.
         * 
         * Reads FAULT_MASK bit 7.
         * @returns
         * 0 = Normal   1 = VIN_UV fault interrupt masked
         */
        bool InputUnderVoltageFault() 
        {
            return ((read8bitRegister(FAULT_MASK) >> 7) & 0x01) == 0; 
        }

        // Check if Input over-voltage event produces INT pulse
        // Return
        //      bool: True if Input over-voltage event produces INT pulse
        bool InputOverVoltageFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 6) & 0x01) == 0; 
        } 

        // Check if Battery over-current event produces INT pulse
        // Return
        //      bool: True if Battery over-current event produces INT pulse
        bool BatteryOverCurrentFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 5) & 0x01) == 0;
        }

        // Check if Battery over-current event produces INT pulse
        // Return
        //      bool: True if Battery over-current event produces INT pulse
        bool BatteryOverVoltageFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 4) & 0x01) == 0;
        }

        // Check if TSHUT event produces INT pulse
        // Return
        //      bool: True if TSHUT event produces INT pulse
        bool ThermalShutdownFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 3) & 0x01) == 0;
        }

        // Check if Timer expired rising edge produces INT pulse
        // Return
        //      bool: True if Timer expired rising edge produces INT pulse
        bool ChargeSafetyTimerFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 2) & 0x01) == 0;
        }

        // Check if DRV_SUP pin fault produces INT pulse
        // Return
        //      bool: True if DRV_SUP pin fault produces INT pulse
        bool DRV_SUPPinFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 1) & 0x01) == 0;
        }




}