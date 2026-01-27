// Write easy test code to test this library with arduino
#include <Wire.h>
#include "BQ25756.h"
#include "test_bq25756_adc.h"


void setup(){
    Serial.begin(9600);
    // Check the wiring I2C to set up test correctly
    Wire.begin();
    Wire.beginTransmission(I2C_BUS_ADDR);
    
    Serial.println("Startig I2C....");


    test_ADC_enable();
    delay(500);
    
    test_enableIAC_ADC();
    delay(500);

    test_enableIBAT_ADC();
    delay(500);

    test_enableVAC_ADC();
    delay(500);

    test_enableVBAT_ADC();
    delay(500);

    test_enableTS_ADC();
    delay(500);

    test_enableVFB_ADC();
    delay(500);

    test_ADC_disable();
    delay(500);
    
    test_disableIAC_ADC();
    delay(500);

    test_disableIBAT_ADC();
    delay(500);

    test_disableVAC_ADC();
    delay(500);

    test_disableVBAT_ADC();
    delay(500);

    test_disableTS_ADC();
    delay(500);

    test_disableVFB_ADC();
    delay(500);

    test_enableAllADCControl();
    delay(500);
  
}


void loop() {

}