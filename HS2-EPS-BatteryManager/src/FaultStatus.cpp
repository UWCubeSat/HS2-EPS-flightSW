
// Class for INT Status
# include "BQ25756.h"
# include "i2c.h"

class FaultStatus: public BQ25756{
    public:

        // Return
        //      bool: True if Input under-voltage event produces INT pulse
        bool InputUnderVoltageFault() 
        {
            return ((read8bitRegister(FAULT_MASK) >> 7) & 0x01) == 0; 
        }

        // Return
        //      bool: True if Input over-voltage event produces INT pulse
        bool InputOverVoltageFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 6) & 0x01) == 0; 
        } 

        // Return
        //      bool: True if Battery over-current event produces INT pulse
        bool BatteryOverCurrentFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 5) & 0x01) == 0;
        }

        // Return
        //      bool: True if Battery over-current event produces INT pulse
        bool BatteryOverVoltageFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 4) & 0x01) == 0;
        }

        // Return
        //      bool: True if TSHUT event produces INT pulse
        bool ThermalShutdownFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 3) & 0x01) == 0;
        }

        // Return
        //      bool: True if Timer expired rising edge produces INT pulse
        bool ChargeSafetyTimerFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 2) & 0x01) == 0;
        }

        // Return
        //      bool: True if DRV_SUP pin fault produces INT pulse
        bool DRV_SUPPinFault()
        {
            return ((read8bitRegister(FAULT_MASK) >> 1) & 0x01) == 0;
        }

        


}