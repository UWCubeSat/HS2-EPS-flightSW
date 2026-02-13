#include <Wire.h>
#include "BQ25756.h"
#include "test_BatteryMonitor.h"

void setup() {
    Serial.begin(9600);

    // Check the wiring I2C to set up test correctly
    Wire.begin();
    Wire.beginTransmission(0x6B);
    
    Serial.println("Starting I2C....");
    
    delay(500);

    enableADCSequence();
    
    delay(500);

    //Test the Monitor functions
    reportStatus();
    delay(500);
    printVBAT_LOWV();
}

void loop() {
}