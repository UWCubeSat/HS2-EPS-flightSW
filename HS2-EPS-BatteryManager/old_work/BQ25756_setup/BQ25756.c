// This is the code to set up BQ25756 using raspberry pi
/*
#include "BQ25756_reg.h"
#include <stdio.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <stdint.h>
#include <wiringPiI2C.h>

int fd = -1;

int main(void) {
    // Setup I2C
    fd = wiringPiI2CSetup(I2C_BUS_ADDR);
    if (fd == -1) {
        printf("I2C Setup failed.\n");
        return -1;
    }


    return 0;
}

void powerOn() {

} 

uint8_t getCHARGE_STAT()
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

// Get data from register
int chargeVolLimitOperation(char operation, int voltageValue) {
    if (char == 'r') {
        return read8BitRegister(CHARGE_VOLT_LIM);
    } else if (char == 'w') {
        uint8_t registerValue = (voltageValue - 1504) / 2;
        writeRegister(CHARGE_VOLT_LIM, registerValue);
        return read8BitRegister(CHARGE_VOLT_LIM);
    } else {
        return -1;
    }
}

// Offset is 1504 mV, bit steps is 2 mV, value has to be even 1504-1566 mV
int changeVolLimit(int value) {
    uint8_t writeVal = (uint8_t)((value - 1504)/2);
    uint8_t VFB_REG = CHARGE_VOLT_LIM;
    writeRegister(CHARGE_VOLT_LIM, writeVal);
    uint8_t result = read8BitRegister(CHARGE_VOLT_LIM);

    int return_result = (int) result;
    return_result = return_result * 2 + 1504; 
    return return_result;
}

// Range  400mA-20000mA, bit steps is 50 mA
int changeCurrLimit(int value) {
    uint16_t writeVal = ;
    uint8_t highByte = (writeVal >> 8) &  0xFF;
    writeRegister(, );

}

uint16_t read16BitRegister (uint8_t reg) {
    return wiringPiI2CReadReg16 (fd, reg);
}

uint8_t read8BitRegister (uint8_t reg) 
{
    return wiringPiI2CReadReg8(fd, reg);
}

void writeRegister(uint8_t regAdd, uint8_t val) {
    wiringPiI2CWriteReg8(fd, regAdd, val);
}






*/