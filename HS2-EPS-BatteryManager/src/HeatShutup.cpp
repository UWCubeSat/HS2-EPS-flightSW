#include <Wire.h>
#include "BQ25756_reg.h"

//Represents the levels of TS
enum TS_LVL {
    TS_NORMAL,
    TS_WARM,
    TS_COOL,
    TS_COLD,
    TS_HOT
};

// Obtain the status of the Thermal Shutdown
// Returns status based off of the set JEITA levels 
// This IC Jumps T4 so we will just return 4 as T5
TS_LVL readTS_STAT(){
    uint8_t readVal = read8bitRegister(CHARGER_STATUS_2);
    readVal &= 0x70;
    TS_LVL tsTemp;
    switch(readVal >> 4)
    {
    case 0:
        tsTemp = TS_NORMAL;
        break;
    case 1:
        tsTemp = TS_WARM;
        break;

    case 2:
        tsTemp = TS_COOL;
        break;
    
    case 3:
        tsTEMP = TS_COLD;
        break;
    
    case 4:
        tsTEMP = TS_HOT;
        break;

    default:
        return -1;
    }
    return tsTemp;
}

// Measures the voltage on the TS Pin and
// returns as percentage of REGN
float TS_ADC (){
    uint16_t tsADC = read16BitRegister(TS_ADC);
    float tsPercentage = (tsADC / 1024) * 100;
    return tsPercentage;
}

//Enables the current JEITA profile
void JEITA_enable (){
    uint8_t writeVal = read8BitRegister(0x1C);
    writeVal |= (0x02);
    writeRegister(0x1C, writeVal);
} 

//Disables the current JEITA profile
void JEITA_disable (){
    uint8_t writeVal = read8BitRegister(0x1C);
    writeVal &= ~(0x02);
    writeRegister(0x1C, writeVal);
}

//Enables the Thermosistor
void TS_enable (){
     uint8_t writeVal = read8BitRegister(0x1C);
     writeVal |= (0x01);
     writeRegister(0x1C, writeVal);
}

//Disables the Thermosistor
void TS_disable (){
    uint8_t writeVal = read8BitRegister(0x1C);
    writeVal &= ~(0x01);
    writeRegister(0x1C, writeVal);
}

//
// uint8_t configure_TS_Charging_Threshold_Control (){
    
// }