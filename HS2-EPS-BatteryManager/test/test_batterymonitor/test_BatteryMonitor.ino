#include <Wire.h>
#include "BQ25756.h"
#include "BatteryMonitor.h"
#include "test_BatteryMonitor.h"
BQ25756 bq;

void reportStatus();
void printVBAT_LOWV();

void setup() {
    Serial.begin(9600);

    Wire.begin();
    Serial.println("Starting I2C....");
    delay(500);

    bq.adc.enableAllADCControl();
    bq.adc.setADCContinuous();   // FIXED
    bq.adc.enableADC();

    delay(500);

    reportStatus();
    delay(500);
    printVBAT_LOWV();
    enableCharging();
    delay(500);
}

void loop() { 
    reportStatus();
    delay(500);
    printVBAT_LOWV();
    delay(500);
    }