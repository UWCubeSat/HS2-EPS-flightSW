// This is the code to test for changing threshold value
// This is the setting code for BQ25756
#include <Wire.h>
#include "BQ25756_ard.h"
#include "changeThreshold.h"
#include "ADC_control.h"
#include "TS_charge_control.h"


float Vbat_ADC_read;
// 
// IBAT ADC, IAC ADC, look into how to change
// IAC_DPM, ICHG REG, 
// Change 

void setup(){
    // Start Serial communication
    Serial.begin(9600);
    Wire.begin();
    Wire.beginTransmission(I2C_BUS_ADDR);
    Serial.println("Startig I2C....");
    delay(50);

    // Enable ADC
    enableADC(); 
    Serial.println("ADC enabled");
    // Get the initial charging status
    uint8_t charging_status = getCHARGE_STAT();

    delay(50);

    // Get the current ADC Value: VAC, VBAT, Vfb
    void getProperties();
    delay(100);

    // Change Vfb_reg, value should be in range 1504mV-1566mV,
    int currVfb_reg = setVfb_reg(1566);
    Serial.print("Vfb_reg: ");
    Serial.print(currVfb_reg);
    Serial.println(" mV");

    // Change Vrechg could be set 93.0 - 97.6% of Vfb_reg
    // percentage option: 1 - 93.0%,  2 - 94.3%,  3 - 95.2%, 4 - 97.6%
    int VrechgOption = setVrechg(1);
    Serial.print("Vrechg option: ");
    Serial.println(VrechgOption);

    // Set Vbat_lowv % of Vfb_reg
    // percentage option: 1 - 30%,  2 - 55%,  3 - 66.7%, 4 - 71.4%
    // Return: the number of option set
    int currVbat_low = setVbat_lowv(4);
    Serial.print("Vbatlowv option: ");
    Serial.println(currVbat_low);

    // Sets ICHG_REG (charge current limit) in REG0x02/0x03
    // Valid range: 400 mA to 20000 mA, in 50 mA steps
    int currICHG_reg = setIchg(20000);
    Serial.print("ICHG_REG: ");
    Serial.print(currICHG_reg);
    Serial.println(" mA");

    // Write register 
    uint8_t temp = read8BitRegister(0x2B);
    uint8_t new_val = temp | 0x80;
    writeRegister(0x2B, new_val);

    uint8_t temp1 = read8BitRegister(0x17);
    //uint8_t new_val1 = temp1 & 0x00;
    uint8_t new_val1 = temp1 | 0xFF;
    writeRegister(0x17, new_val1);
    uint16_t tempa = read8BitRegister(0x07);
    uint16_t tempb = read8BitRegister(0x06);
    uint16_t resultab = (tempa << 8) | tempb;
    uint16_t iacDPM = (resultab >> 2) & 0xFF; 
    Serial.print("DPM: ");
    Serial.println(iacDPM);
    enableDPM();
    //setreadIAC_DPM();
    


}

void loop() {
    uint8_t charging_status = getCHARGE_STAT();

    void getProperties();
    int currICHG_reg = setIchg(20000);
    Serial.print("ICHG_REG: ");
    Serial.print(currICHG_reg);
    Serial.println(" mA");
    test();
    enableDPM();
    setreadIAC_DPM();
    uint8_t readCurr = read8BitRegister(0x18);
    uint16_t low= read8BitRegister(0x06);
    uint16_t high= read8BitRegister(0x07);
    uint16_t readVal =  (high << 8) | low;
    readVal = readVal >> 2;
    Serial.println(readVal);
    Serial.println(readCurr);
    
    delay(1000);
}

void test() {
    float IAC_val = readIAC_ADC();
    float IBAT_val = readIBAT_ADC();
    float VAC_val = readVAC_ADC();
    float VBAT_val = readVBAT_ADC();
    float TS_val = readTS_ADC(1.0);
    float VFB_val = readVFB_ADC();
    float IAC_DPM_read = readIAC_DPM();
    
    Serial.print("IAC: ");
    Serial.println(IAC_val);
    Serial.print("IBAT: ");
    Serial.println(IBAT_val);
    Serial.print("VAC: ");
    Serial.println(VAC_val);
    Serial.print("VBAT: ");
    Serial.println(VBAT_val);
    Serial.print("TS: ");
    Serial.println(TS_val);
    Serial.print("VFB: ");
    Serial.println(VFB_val);
    Serial.print("IAC_DPM: ");
    Serial.println(IAC_DPM_read);
    Serial.println("");
    



}

void enableDPM() {
    uint16_t curr = read8BitRegister(0x18);
    writeRegister(0x18, 0x80);
    //int16_t curr2 = read8BitRegister(0x21);
    //writeRegister(0x21, )
}
float readIAC_DPM() {
    uint16_t low= read8BitRegister(0x06);
    uint16_t high= read8BitRegister(0x07);
    uint16_t readVal =  (high << 8) | low;
    uint16_t raw_value = (readVal >> 2) & 0xFF;
    float current_limit_mA = (float) raw_value * 50.0f;
    return current_limit_mA;
}   

void setreadIAC_DPM() {
    writeRegister(0x07, 0x00);
    writeRegister(0x06, 0x20);
}
