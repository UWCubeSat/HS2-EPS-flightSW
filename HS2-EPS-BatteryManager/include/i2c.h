#ifndef I2C_H
#define I2C_H

// Use I2C library for Arduino
#include <Wire.h>
#include "BQ25756_reg.h"

uint8_t  read8bitRegister(uint8_t reg);
uint16_t read16BitRegister(uint8_t reg);
void     writeRegister(uint8_t reg, uint8_t val);

// Register bit helpers — operate on a single named register
void setBit(uint8_t reg, uint8_t mask);   // set bits in mask to 1
void clearBit(uint8_t reg, uint8_t mask); // set bits in mask to 0
bool readBit(uint8_t reg, uint8_t mask);  // true if any bit in mask is set

#endif