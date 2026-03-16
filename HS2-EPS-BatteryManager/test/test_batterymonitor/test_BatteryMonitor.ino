#include <Wire.h>
#include "BQ25756.h"
#include "test_BatteryMonitor.h"

// This is helper function to use printf in ino code
int serial_putchar(char c, FILE* f)
{
    Serial.write(c);
    return c;
}

FILE serial_stdout;

void setup() {
    Serial.begin(9600);


    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_stdout;

    printf("printf now works!\n");
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
      delay(500);

    //Test the Monitor functions
    reportStatus();
    delay(500);
    printVBAT_LOWV();
}