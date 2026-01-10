// Class for INT Status
#include "FaultStatus.h"
#include "BQ25756.h"
#include "i2c.h"

class FaultStatus: public BQ25756{
    public:

        // FAULT_MASK: if unmasked, fault will geberate INT pulse, if masked, fault will not generate INT pulse

        /**
         * @brief Check if Input Under-Voltage INT masked.
         * 
         * Reads FAULT_MASK bit 7.
         * @returns
         * 1 = Input under-voltage event produces INT pulse, 0 = otherwise
         */
        bool InputUnderVoltageFaultMask() 
        {
            return ((read8bitRegister(FAULT_MASK) >> 7) & 0x01) == 0; 
        }

        /**
         * @brief Check if Input over-voltage INT masked.
         * 
         * Reads FAULT_MASk bit 6.
         * @returns
         * 1 = Input over-voltage event produces INT pulse, 0 = otherwise
        */
        bool InputOverVoltageFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 6) & 0x01) == 0; 
        } 

        /**
         * @brief Check if Battery over-current INT masked.
         * 
         * Reads FAULT_MASK bit 5.
         * @returns
         * 1 = Battery over-current event produces INT pulse, 0 = otherwise
         */
        bool BatteryOverCurrentFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 5) & 0x01) == 0;
        }

        /**
         *  @brief Check if Battery over-voltage INT masked
         * 
         *  Reads FAULT_MASK bit 4.
         *  @returns
         *  1 = Battery over-voltage event produces INT pulse, 0 = otherwise
         */
        bool BatteryOverVoltageFaultMask()
        {
            return ((read8bitRegister(FAULT_MASK) >> 4) & 0x01) == 0;
        }

        /**
         *  @brief Check if Thermal shutdown INT masked
         * 
         *  Reads FAULT_MASK bit 3
         *  @returns
         *  1 = TSHUT event produces INT pulse, 0 = otherwise
         */
        bool ThermalShutdownFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 3) & 0x01) == 0;
        }

        /**
         *  @brief Check if Timer expired rising edge produces INT pulse
         * 
         *  Reads FAULT_MASK bit 2
         *  @returns
         *  1 = Timer expired rising edge produces INT pulse, 0 = otherwise
         */
        bool ChargeSafetyTimerFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 2) & 0x01) == 0;
        }

        // Check if DRV_SUP pin fault produces INT pulse
        // Return
        //      bool: True if DRV_SUP pin fault produces INT pulse
        /**
         *  @brief Check if DRV_SUP pin fault produces INT pulse
         * 
         *  Reads FAULT_MASK bit 1
         *  @returns
         *  1 = DRV_SUP pin fault produces INT pulse, 0 = otherwise
         */
        bool DRV_SUPPinFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 1) & 0x01) == 0;
        }




}