// Class for INT Status
#include "FaultStatus.h"
#include "BQ25756.h"
#include "i2c.h"

class FaultStatus: public BQ25756{
    public:

        /**
         * @brief Check Input Under-Voltage fault status.
         * 
         * Reads FAULT_MASK bit 7.
         * @returns
         * 0 = Input under-voltage event produces INT pulse
         * 1 = Input under-voltage event does not produce INT pulse
         */
        bool InputUnderVoltageFault() 
        {
            return ((read8bitRegister(FAULT_MASK) >> 7) & 0x01) == 0; 
        }

        /**
         * @brief Check if Input over-voltage event produces INT pulse
         * 
         * Reads FAULT_MASk bit 6.
         * @returns
         * 0 = Input over-voltage event produces INT pulse
         * 1 = Input over-voltage event does not produce INT pulse
        */
        bool InputOverVoltageFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 6) & 0x01) == 0; 
        } 

        // Check if Battery over-current event produces INT pulse
        // Return
        //      bool: True if Battery over-current event produces INT pulse
        /**
         * @brief Check Battery over-current fault status.
         * 
         * Reads FAULT_MASK bit 5.
         * @returns
         * 0 = Normal   1 = VIN_UV fault interrupt masked
         */
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