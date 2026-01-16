// Write easy test code to test this library with arduino
#include <Wire.h>
#include "BQ25756.h"
#include ""

void setup(){
    Serial.begin(9600);
    Wire.begin();
    Wire.beginTransmission(I2C_BUS_ADDR);
    // Instantiate BQ25756
    Serial.println("Startig I2C....");
}


void loop() {


}