// Use Arduino I2C library <Wire.h>
#include "i2c.h"
#include "BQ25756_reg.h"

// Read register and get 8 bits data
uint8_t read8bitRegister(uint8_t reg)
{
    uint8_t data = 0;
    Wire.beginTransmission(I2C_BUS_ADDR);       // START condiiton
    Wire.write(reg);                            // Send target register address
    Wire.endTransmission(false);                // Finish sending address, keeping BUS open
    Wire.requestFrom(I2C_BUS_ADDR, 1);          // Get 1 Byte from SDA (Fetch it into buffer)

    if (Wire.available())                       // If data is stored in buffer
    {
        data = Wire.read();                     // Read data from bbuffer
    } else 
    {
        Serial.println("Data read: FAILED");
    }
    return data;
}

// Read register and get 16 bits data
uint16_t read16bitRegister (uint8_t reg) 
{
    uint16_t data = 0;
    Wire.beginTransmission(I2C_BUS_ADDR);
    Wire.write(reg);

    if (Wire.endTransmission(false) != 0) {
        Serial.println("I2C write failed");
        return 0xFFFF;
    }

    if (Wire.requestFrom((uint8_t)I2C_BUS_ADDR, (uint8_t)2) != 2) {
        Serial.println("I2C read failed");
        return 0xFFFF;
    }

    uint16_t lsb = Wire.read();             // Read first 8bits data [7:0]
    uint16_t msb = Wire.read();             // Read second 8bits data [15:8]
    data = (msb << 8) | lsb;

    return data;
}

// Write 8 bits register with 8 bits data
void writeRegister(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(I2C_BUS_ADDR);
    Wire.write(reg);                            // Send 8 bits address first
    Wire.write(val);                            // Send the value to write
    Wire.endTransmission();
}
