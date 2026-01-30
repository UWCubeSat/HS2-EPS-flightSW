#include "BatteryMonitor.h"

class BQ25756::BatteryMonitor {
    public:

        /*@brief "Stores battery properties in a map for easy acccess"

          @return "Returns a map of battery properties and their values in millivolts or milliamps" 
        */  
        std::map<std::string, int> getProperties() {
            std::map<std::string, int> properties;
            properties["VAC"] = getVac();
            properties["VBAT"] = getVbat();
            properties["VFB"] = getVfb();
            properties["VRECHG"] = readVrechg();
            properties["VBAT_LOWV"] = readVbat_lowv();
            properties["ICHG"] = readIchg();
            properties["IAC"] = getIac();
            properties["IBAT"] = getIbat();
            return properties;
        }

    private:

        //Feedback Voltage
        // Gets VFB ADC reading (Range: 0mV-2047mV)
        // Return:
        //          int: VFB ADC reading (mV)
        int getVfb () 
        {
              uint16_t data = read16BitRegister(VFB_ADC);
              int vfbValue = data * 2;
              return vfbValue;
        }

        //Input Voltage
        //Gets the VAC ADC Reading (Range: 0mV-65534mV)
        //Return:
        //          int: VAC ADC Reading (mv)
        int getVac () 
        {
            uint16_t data = read16BitRegister(VAC_ADC);
            int vacValue = data * 2;
            return vacValue;
        }

        //Battery Voltage
        //Gets the VBAT ADC Reading (Range: 0mV-65534mV)
        //Return:
        //          int: VBAT ADC Reading (mv)
        int getVbat() 
        {
            uint16_t data = read16BitRegister(VBAT_ADC);
            int vbatValue = data * 2;
            return vbatValue;
        }

        //Input Current
        //Gets the IAC ADC Reading (Range: -20000mA - 20000mA)
        //Return:
        //          int: IAC ADC Reading (mA)
        int getIac() 
        {
            int16_t data = (int16_t)read16BitRegister(IAC_ADC);
            int iacValue = (int) data * 8 / 10; 
            return iacValue;
        }

        //Battery Current
        //Gets the IBAT ADC Reading (Range: -20000mA-20000mA)
        //Return:
        //          int: IBAT ADC Reading (mA)
        int getIbat() 
        {
            int16_t data = (int16_t)read16BitRegister(IBAT_ADC);
            int ibatValue = (int) data * 2; 
            return ibatValue;
        }        

        /*FB Voltage Regulation Limit:
        Range: 1504mV-1566mV 
        Bit Step: 2mV
        Offset: 1504mV*/
        int getVfbReg() {
            uint8_t data = read8BitRegister(CHARGE_VOLT_LIM);
            int vfbRegValue = data * 2 + 1504;
            return vfbRegValue;
        }



        //Battery auto-recharge threshold, as percentage of VFB_REG. 
        //Reads CHARGER_CONT register
        int readVrechg() {
            uint8_t regVal = read8BitRegister(CHARGER_CONT);
            int option = (regVal >> 6) & 0x03; // Get the two bits related to Vrechg
            int vfb_reg = getVfbReg(); // Read Vfb_reg to get the base voltage
            float percentage = 0.0;

            switch (option) {
                case 0:
                    percentage = 0.93; // 93.0%
                    break;
                case 1:
                    percentage = 0.943; // 94.3%
                    break;
                case 2:
                    percentage = 0.952; // 95.2%
                    break;
                case 3:
                    percentage = 0.976; // 97.6%
                    break;
                default:
                    return -1; // Invalid option
            }

            int vrechg = vfb_reg * percentage; // Calculate Vrechg based on Vfb_reg and percentage
            return vrechg;
        }



        //Battery threshold for PRECHG to FASTCHG transition, as percentage of VFB_REG
        //Reads PRECHARGE_AND_TERMINATION_CONTROL register
        int readVbat_lowv() {
            uint8_t regVal = read8BitRegister(PRECHARGE_TERM_CONT);
            int option = (regVal >> 1) & 0x03; // Get the two bits related to Vbat_lowv
            int vfb_reg = getVfbReg(); // Read Vfb_reg to get the base voltage
            float percentage = 0.0;

            switch (option) {
                case 0:
                    percentage = 0.30; // 30%
                    break;
                case 1:
                    percentage = 0.55; // 55%
                    break;
                case 2:
                    percentage = 0.667; // 66.7%
                    break;
                case 3:
                    percentage = 0.714; // 71.4%
                    break;
                default:
                    return -1; // Invalid option
            }

            int vbat_lowv = vfb_reg * percentage; // Calculate Vbat_lowv based on Vfb_reg and percentage
            return vbat_lowv;
        }


        //Fast Charge Current Regulation Limit with 5mΩ RBAT_SNS (battery sense resistor):
        //Actual charge current is the lower of ICHG_REG and ICHG pin
        //Range: 400mA-20000mA 
        //Bit Step: 50mA
        int readIchg() {
            // I2C REG0x03=[15:8], I2C REG0x02=[7:0]
            uint8_t regLsb = read16BitRegister(0x02);  // REG0x02 (LSB)
            uint8_t regMsb = read16BitRegister(0x03);  // REG0x03 (MSB)
            uint16_t reg = ((uint16_t)regMsb << 8) | regLsb;

            // Extract the charge current value (bits 10:2)
            uint16_t ichg_value = (reg >> 2) & 0x1FF;

            // Convert to current in mA (50mA per bit step)
            int ichg = ichg_value * 50;
            return ichg;
        }

        enum class ChargingStatus : uint8_t {
            NOT_CHARGING = 0x00,
            TRICKLE_CHARGE = 0x01,
            PRE_CHARGE = 0x02,
            FAST_CHARGE = 0x03,
            TAPER_CHARGE = 0x04,
            RESERVED = 0x05,
            TOP_OFF_TIMER_ACTIVE = 0x06,
            CHARGE_TERMINATION_DONE = 0x07
        };

        //Gets current charging status
        //Reading from CHARGER_STATUS_1 register
        ChargingStatus getChargingStatus()  
        {
            uint8_t data = read8BitRegister(CHARGER_STATUS_1);
            uint8_t bit2_0 = data & 0x07;
            ChargingStatus status = static_cast<ChargingStatus>(bit2_0);
            return status;
        }
};