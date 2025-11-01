/* This contains the basic function to read register write register via I2C
*/
#include <Wire.h>
#include "BQ25756_reg.h"

uint8_t read8bitRegister(uint8_t reg);
uint16_t read16BitRegister (uint8_t reg);
void writeRegister(uint8_t reg, uint8_t val);
