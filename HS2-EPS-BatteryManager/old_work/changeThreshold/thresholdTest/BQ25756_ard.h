#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "BQ25756_reg.h"

void getProperties();
uint32_t getVoltage_ADC ();
void printVBAT_LOWV();
uint32_t getVfb_ADC ();
uint32_t getVBAT_ADC();
uint16_t getIBAT_ADC();
int16_t convertIbatADC(uint16_t adcValue);
int readVfb_reg();
int readVrechg();
int readVbat_lowv();
int readIchg();
uint8_t getCHARGE_STAT();
uint16_t read16BitRegister (uint8_t reg);
uint8_t read8BitRegister (uint8_t reg);
void writeRegister(uint8_t reg, uint8_t val);

