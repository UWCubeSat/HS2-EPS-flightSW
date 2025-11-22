#include "BatteryMonitor.h"

class BQ25756::BatteryMonitor {
    public:
        //TODO: Raspberry pi doesn't play well with Serial.print. Use dictionary, maps, etc...
        void reportStatus() {
            int vac = getVac();
            Serial.print("VAC ADC Voltage: ");
            Serial.print(vac);
            Serial.println(" mV");

            int vbat = getVbat();
            Serial.print("VBAT ADC Voltage: ");
            Serial.print(vbat);
            Serial.println(" mV");

            int vfb = getVfb();
            Serial.print("Vfb ADC Voltage: ");
            Serial.print(vfb);
            Serial.println(" mV");

            int vrechg = readVrechg();
            Serial.print("Vrechg Voltage: ");
            Serial.print(vrechg);
            Serial.println(" mV");

            int vbat_lowv = readVbat_lowv();
            Serial.print("Vbat Low Voltage Threshold: ");
            Serial.print(vbat_lowv);
            Serial.println(" mV");

            int ichg = readIchg();
            Serial.print("Charge Current Limit: ");
            Serial.print(ichg);
            Serial.println(" mA");

            int iac = getIac();
            Serial.print("IAC ADC Current: ");
            Serial.print(iac);
            Serial.print(" mA");

            int ibat = getIbat();
            Serial.print("Ibat ADC Current: ");
            Serial.print(ibat);
            Serial.println(" mA");
        }

        void printVBAT_LOWV() {
        uint8_t data = read8BitRegister(PRECHARGE_TERM_CONT);
        uint8_t bit2_1Value = (data & 0x06) >> 1;
        Serial.print("VBAT_LOW is");
        Serial.println(bit2_1Value);
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
            return value;
        }

        
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

        int readIchg() {
            uint8_t regLsb = read8BitRegister(0x02);  // REG0x02 (LSB)
            uint8_t regMsb = read8BitRegister(0x03);  // REG0x03 (MSB)
            uint16_t reg = ((uint16_t)regMsb << 8) | regLsb;

            // Extract the charge current value (bits 10:2)
            uint16_t ichg_value = (reg >> 2) & 0x1FF;

            // Convert to current in mA (50mA per bit step)
            int ichg = ichg_value * 50;
            return ichg;
        }

        //TODO: Implement this using enums here, println in report_status
        uint8_t get_chargingStatus()  //The current charging status
        {
            uint8_t data = read8BitRegister(CHARGER_STATUS_1);
            uint8_t bit2_0 = data & 0x07;
            Serial.print("Charging Status: ");
            switch (bit2_0)
            {
                case 0x00:
                    Serial.println("Not Charging");
                    break;
                case 0x01:
                    Serial.println("Trickle Charge (VFB < VBAT_SHORT)");
                    break;
                case 0x02:
                    Serial.println("Pre-charge (VBAT_SHORT < VFB < VBAT_LOWV)");
                    break;
                case 0x03:
                    Serial.println("Fast-charge (CC mode)");
                    break;
                case 0x04:
                    Serial.println("Taper Charge (CV mode)");
                    break;
                case 0x05:
                    Serial.println("Reserved");
                    break;
                case 0x06:
                    Serial.println("Top-off Timer Active Charging");
                    break;
                case 0x07:
                    Serial.println("Charge Termination Done");
                    break;
            }

            return bit2_0;
        }
};