// This is the code to define the register
// This is the setting code for BQ25756
#include <Wire.h>
#include "BQ25756.h"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(I2C_BUS_ADDR);
  Serial.println("Startig I2C....");
}

void loop() {
  // put your main code here, to run repeatedly:
  getProperties();
  uint8_t charging_status = getCHARGE_STAT();
  //printVBAT_LOWV();
  delay(5000);
}
