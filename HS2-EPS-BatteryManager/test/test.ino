// Write easy test code to test this library with arduino
#include <Wire.h>
#include "BQ25756.h"


BQ25756 bq;

void setup(){
    Serial.begin(9600);
    Wire.begin();
    Wire.beginTransmission(I2C_BUS_ADDR);
    // Instantiate BQ25756
    Serial.println("Startig I2C....");
    bq.getStatus();
    bq.adc.enableADC();
    

    bool adcEnabled = bq.adc.ADCEnabled();
    if (adcEnabled){
      Serial.println("ADC is enabled");
    } else {
      Serial.println("ADC is disabled");
    }

}


void loop() {
  bool adcEnabled = bq.adc.ADCEnabled();
  bq.getStatus();
    if (adcEnabled){
      Serial.println("ADC is enabled");
    } else {
      Serial.println("ADC is disabled");
    }

  delay(500);

}