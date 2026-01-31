#include <Wire.h>
#include "BQ25756_reg.h"



// Represents temperature levels of TS
enum BQ25756::HeatShutup::TS_LVL{
    TS_NORMAL,
    TS_WARM,
    TS_COOL,
    TS_COLD,
    TS_HOT,
    TS_INVALID
};

// Represents the four selectable TS percentages for T5
enum BQ25756::HeatShutup::TS_T5_prcnt{
    T5_41p2,
    T5_37p7,
    T5_34p375,
    T5_31p25
};

// Represents the four selectable TS percentages for T3
enum BQ25756::HeatShutup::TS_T3_prcnt{
    T3_48p4,
    T3_44p8,
    T3_41p2,
    T3_37p7
};

// Represents the four selectable TS percentages for T2
enum HeatShutup::TS_T2_prcnt{
    T2_71p1,
    T2_68p4,
    T2_65p5,
    T2_62p4
};

// Represents the four selectable TS percentages for T1
enum HeatShutup::TS_T1_prcnt{
    T1_77p15,
    T1_75p32,
    T1_73p25,
    T1_71p1
};

// Obtain the status of the Thermal Shutdown
// Returns 
//         TS_LVL: Status based off of the set JEITA levels
// 
// This IC Jumps T4 so we will just return 4 as T5
TS_LVL HeatShutup::readTS_STAT(){
    TS_LVL tsTemp;
    uint8_t readVal = read8bitRegister(CHARGER_STATUS_2);
    readVal &= 0x70;
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
            tsTemp = TS_COLD;
            break;
    
        case 4:
            tsTemp = TS_HOT;
            break;

        default:
            tsTemp = TS_INVALID;
            break;
    }
    return tsTemp;
}

// Measures the voltage on the TS Pin and
// Return:
//          float: Voltage on TS Pin as percentage of REGN
float HeatShutup::TS_ADC (){
    uint16_t tsADC = read16BitRegister(TS_ADC);
    float tsPercentage = (tsADC / 1024.0f) * 100;
    return tsPercentage;
}

// Enables the current JEITA profile
void HeatShutup::JEITA_enable (){
    uint8_t readVal = read8BitRegister(CHARGE_REGION_CONT);
    uint8_t writeVal = readVal|(0x02);
    writeRegister(CHARGE_REGION_CONT, writeVal);
} 

// Disables the current JEITA profile
void HeatShutup::JEITA_disable (){
    uint8_t readVal = read8BitRegister(CHARGE_REGION_CONT);
    uint8_t writeVal = readVal & ~(0x02);
    writeRegister(CHARGE_REGION_CONT, writeVal);
}

// Enables TS pin function control
// This could applies to forward charging and reverse discharging modes
void HeatShutup::TS_enable (){
    uint8_t readVal = read8BitRegister(CHARGE_REGION_CONT);
    uint8_t writeVal = readVal | (0x01);
    writeRegister(CHARGE_REGION_CONT, writeVal);
}

//Disables the Thermistor
void HeatShutup::TS_disable (){
    uint8_t readVal = read8BitRegister(CHARGE_REGION_CONT);
    uint8_t writeVal = readVal & ~(0x01);
    writeRegister(CHARGE_REGION_CONT, writeVal);
}

// Adjusts the charging percentages of the T5 zone from four options
// to adjust the temperature zone
void HeatShutup::configure_TS_T5_Charging_Threshold (TS_T5_prcnt userInput){
    uint8_t readVal = read8bitRegister(CHARGE_THRESH_CONT);
    uint8_t writeVal = readVal;
    switch(userInput)
    {
        case T5_41p2:
            writeVal &= 0x3F;
            break;

        case T5_37p7:
            writeVal = (writeVal & 0x3F) | 0x40;
            break;

        case T5_34p375:
            writeVal = (writeVal & 0x3F) | 0x80;
            break;

        case T5_31p25:
            writeVal = (writeVal & 0x3F)  | 0xC0;
            break;
    }
    writeRegister (CHARGE_THRESH_CONT, writeVal);
}

// Adjusts the charging percentages of the T3 zone from four options
// to adjust the temperature zone
void HeatShutup::configure_TS_T3_Charging_Threshold (TS_T3_prcnt userInput){
    uint8_t readVal = read8bitRegister(CHARGE_THRESH_CONT);
    uint8_t writeVal = readVal;
    switch(userInput)
    {
        case T3_48p4:
            writeVal &= 0xCF;
            break;

        case T3_44p8:
            writeVal = (writeVal & 0xCF) | 0x10;
            break;

        case T3_41p2:
            writeVal = (writeVal & 0xCF) | 0x20;
            break;

        case T3_37p7:
            writeVal = (writeVal & 0xCF)  | 0x30;
            break;
    }
    writeRegister (CHARGE_THRESH_CONT, writeVal);
}

// Adjusts the charging percentages of the T2 zone from four options
// to adjust the temperature zone
void HeatShutup::configure_TS_T2_Charging_Threshold (TS_T2_prcnt userInput){
    uint8_t readVal = read8bitRegister(CHARGE_THRESH_CONT);
    uint8_t writeVal = readVal;
    switch(userInput)
    {
        case T2_71p1:
            writeVal &= 0xF3;
            break;

        case T2_68p4:
            writeVal = (writeVal & 0xF3) | 0x04;
            break;

        case T2_65p5:
            writeVal = (writeVal & 0xF3) | 0x08;
            break;

        case T2_62p4:
            writeVal = (writeVal & 0xF3)  | 0x0C;
            break;
    }
    writeRegister (CHARGE_THRESH_CONT, writeVal);
}

// Adjusts the charging percentages of the T1 zone from four options
// to adjust the temperature zone
void HeatShutup::configure_TS_T1_Charging_Threshold (TS_T1_prcnt userInput){
    uint8_t readVal = read8bitRegister(CHARGE_THRESH_CONT);
    uint8_t writeVal = readVal;
    switch(userInput)
    {
        case T1_77p15:
            writeVal &= 0xFC;
            break;

    case T1_75p32:
        writeVal = (writeVal & 0xFC) | 0x01;
        break;

    case T1_73p25:
        writeVal = (writeVal & 0xFC) | 0x02;
        break;

    case T1_71p1:
        writeVal = (writeVal & 0xFC)  | 0x03;
        break;
    }
    writeRegister (CHARGE_THRESH_CONT, writeVal);
}
