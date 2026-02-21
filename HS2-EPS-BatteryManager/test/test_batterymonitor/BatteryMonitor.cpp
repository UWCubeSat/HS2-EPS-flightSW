#include "BatteryMonitor.h"
        /**
     * @brief Convert ChargingStatus enum to a string.
     *
     * This function maps each ChargingStatus enum value to a constant string
     * for Serial printing.
     *
     * @param status  ChargingStatus enum value
     * @return const char*  String of the charging state
     */
    const char* BQ25756::BatteryMonitor::toString(BQ25756::BatteryMonitor::ChargingStatus status) {
      switch (status) {    
           case ChargingStatus::NOT_CHARGING:
                  return("Not Charging");
           case ChargingStatus::TRICKLE_CHARGE:
                  return("Trickle Charge");
           case ChargingStatus::PRE_CHARGE:
                  return("Pre-Charge");
           case ChargingStatus::FAST_CHARGE:
                  return("Fast Charge");
           case ChargingStatus::TAPER_CHARGE:
                  return("Taper Charge");
           case ChargingStatus::RESERVED:
                  return("Reserved");
           case ChargingStatus::TOP_OFF_TIMER_ACTIVE:
                  return("Top-Off Timer Active");
           case ChargingStatus::CHARGE_TERMINATION_DONE:
                  return("Charge Termination Done");
            default:
                  return("Unknown Status");

      }
    }
        /*@brief "Stores battery properties in a map for easy acccess"

          @return "Returns a map of battery properties and their values in millivolts or milliamps" 
        */  
        void BQ25756::BatteryMonitor::getProperties(int properties[8]) {
            properties[0] = getVac();      // VAC
            properties[1] = getVbat();     // VBAT
            properties[2] = getVfb();      // VFB
            properties[3] = readVrechg();  // VRECHG
            properties[4] = readVbat_lowv(); // VBAT_LOWV
            properties[5] = readIchg();    // ICHG
            properties[6] = getIac();      // IAC
            properties[7] = getIbat();     // IBAT
        }


        //Feedback Voltage
        // Gets VFB ADC reading (Range: 0mV-2047mV)
        // Return:
        //          int: VFB ADC reading (mV)
        int BQ25756::BatteryMonitor::getVfb () 
        {
              uint16_t data = read16BitRegister(VFB_ADC);
              int vfbValue = data;
              return vfbValue;
        }

        //Input Voltage
        //Gets the VAC ADC Reading (Range: 0mV-65534mV)
        //Return:
        //          int: VAC ADC Reading (mv)
        int BQ25756::BatteryMonitor::getVac () 
        {
            uint16_t data = read16BitRegister(VAC_ADC);
            int vacValue = data * 2;
            return vacValue;
        }

        //Battery Voltage
        //Gets the VBAT ADC Reading (Range: 0mV-65534mV)
        //Return:
        //          int: VBAT ADC Reading (mv)
        int BQ25756::BatteryMonitor::getVbat() 
        {
            uint16_t data = read16BitRegister(VBAT_ADC);
            int vbatValue = (int)data * 2;
            return vbatValue;
        }

        //Input Current
        //Gets the IAC ADC Reading (Range: -20000mA - 20000mA)
        //Return:
        //          int: IAC ADC Reading (mA)
        int BQ25756::BatteryMonitor::getIac() 
        {
            int16_t data = (int16_t)read16BitRegister(IAC_ADC);
            int iacValue = (int) data * 8 / 10; 
            return iacValue;
        }

        //Battery Current
        //Gets the IBAT ADC Reading (Range: -20000mA-20000mA)
        //Return:
        //          int: IBAT ADC Reading (mA)
        int BQ25756::BatteryMonitor::getIbat() 
        {
            int16_t data = (int16_t)read16BitRegister(IBAT_ADC);
            int ibatValue = (int) data * 2; 
            return ibatValue;
        }        

        /*FB Voltage Regulation Limit:
        Range: 1504mV-1566mV 
        Bit Step: 2mV
        Offset: 1504mV*/
        int BQ25756::BatteryMonitor::getVfbReg() {
            uint8_t data = read8bitRegister(CHARGE_VOLT_LIM);
            int vfbRegValue = data * 2 + 1504;
            return vfbRegValue;
        }



        //Battery auto-recharge threshold, as percentage of VFB_REG. 
        //Reads CHARGER_CONT register
        int BQ25756::BatteryMonitor::readVrechg() {
            uint8_t regVal = read8bitRegister(CHARGER_CONT);
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
        int BQ25756::BatteryMonitor::readVbat_lowv() {
            uint8_t regVal = read8bitRegister(PRECHARGE_TERM_CONT);
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
        int BQ25756::BatteryMonitor::readIchg() {
            // I2C REG0x03=[15:8], I2C REG0x02=[7:0]
            uint8_t regLsb = read8bitRegister(0x02);  // REG0x02 (LSB)
            uint8_t regMsb = read8bitRegister(0x03);  // REG0x03 (MSB)
            uint16_t reg = ((uint16_t)regMsb << 8) | regLsb;

            // Extract the charge current value (bits 10:2)
            uint16_t ichg_value = (reg >> 2) & 0x1FF;

            // Convert to current in mA (50mA per bit step)
            int ichg = ichg_value * 50;
            return ichg;
        }

        //Gets current charging status
        //Reading from CHARGER_STATUS_1 register
        BQ25756::BatteryMonitor::
        ChargingStatus BQ25756::BatteryMonitor::getChargingStatus()  
        {
            uint8_t data = read8bitRegister(CHARGER_STATUS_1);
            uint8_t bit2_0 = data & 0x07;
            ChargingStatus status = static_cast<ChargingStatus>(bit2_0);
            return status;
        }
