#include "BatteryMonitor.h"

void report_status() {
    uint32_t voltage_ADC = get_Vac();
    Serial.print("VAC ADC Voltage: ");
    Serial.print((float) voltage_ADC);
    Serial.println(" mV");

    uint32_t Vbat = get_Vbat();
    Serial.print("VBAT ADC Voltage: ");
    Serial.print((float) Vbat);
    Serial.println(" mV");

    uint32_t Vfb = get_Vfb();
    Serial.print("Vfb ADC Voltage: ");
    Serial.print((float) Vfb);
    Serial.println(" mV");

    int vrechg = readVrechg();
    Serial.print("Vrechg Voltage: ");
    Serial.print((float) vrechg);
    Serial.println(" mV");

    int vbat_lowv = readVbat_lowv();
    Serial.print("Vbat Low Voltage Threshold: ");
    Serial.print((float) vbat_lowv);
    Serial.println(" mV");

    int ichg = readIchg();
    Serial.print("Charge Current Limit: ");
    Serial.print(ichg);
    Serial.println(" mA");

    int32_t iac = get_Iac();
    Serial.print("IAC ADC Current: ");
    Serial.print((float) iac);
    Serial.print(" mA");

    int32_t ibat = get_Ibat();
    Serial.print("Ibat ADC Current: ");
    Serial.print((float) ibat);
    Serial.print(" mA");
}

void printVBAT_LOWV() {
  uint8_t data = read8BitRegister(PRECHARGE_TERM_CONT);
  uint8_t bit2_1Value = (data & 0x06) >> 1;
  Serial.print("VBAT_LOW is");
  Serial.println(bit2_1Value);
}

uint32_t get_Vfb () 
{
  uint16_t data = read16BitRegister(VAC_ADC);
  uint32_t voltage = data * 2;
  return voltage;
}


uint32_t get_Vac () //Gets the input voltage
{
    uint16_t data = read16BitRegister(VAC_ADC);
    uint32_t voltage = data * 2;
    return voltage;
}

uint32_t get_Vbat() //Gets the battery voltage
{
  uint16_t data = read16BitRegister(VBAT_ADC);
  uint32_t voltage = data * 2;
  return voltage;
}

int32_t get_Iac() //Gets the input current
{
    int16_t data = (int16_t)read16BitRegister(IAC_ADC);
    int32_t current = (int32_t) data * 8 / 10; 
    return current;
}

int32_t get_Ibat() //Gets the battery current
{
    int16_t data = (int16_t)read16BitRegister(IBAT_ADC);
    int32_t current = (int32_t) data * 2; 
    return current;
}
// ----------------------Read Value functions---------------------------
int readVfb_reg() {
    uint8_t regVal = read8BitRegister(CHARGE_VOLT_LIM);
    int value = regVal * 2 + 1504;
    return value;
}

int readVrechg() {
    uint8_t regVal = read8BitRegister(CHARGER_CONT);
    int option = (regVal >> 6) & 0x03; // Get the two bits related to Vrechg
    int vfb_reg = readVfb_reg(); // Read Vfb_reg to get the base voltage
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
    int vfb_reg = readVfb_reg(); // Read Vfb_reg to get the base voltage
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
