// Write easy test code to test this library with arduino
#include <Wire.h>
#include "BQ25756.h"
#include "test_bq25756_adc.h"

// This is helper function to use printf in ino code
int serial_putchar(char c, FILE* f)
{
    Serial.write(c);
    return c;
}

FILE serial_stdout;


void setup(){
    Serial.begin(9600);

    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_stdout;

    printf("printf now works!\n");

    // Check the wiring I2C to set up test correctly
    Wire.begin();
    Wire.beginTransmission(0x6B);
    
    Serial.println("Startig I2C....");
    
    delay(500);

    // Read current ADC control
    printf("current state\n");
    print_state();

    test_ADC_enable();
    delay(500);

    /*------------- Test disable functions---------------*/
    printf("Test disable functions\n");
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

    
    /*------------- Test enable functions---------------*/
    printf("Test enble functions\n");
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


    /*------------- Test enable all adc control functions---------------*/
    test_enableAllADCControl();
    delay(500);
  
}


void loop() {
    

}
