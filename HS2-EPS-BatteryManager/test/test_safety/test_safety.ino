// Write easy test code to test this library with arduino
#include <Wire.h>
#include "BQ25756.h"
#include "test_safety.h"

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
    delay(500);
    test_disableWatchdogTimerControl();
    delay(500);
    test_EnableTSPinFunctionControl();
    delay(500);
    test_enableMPPT();
    delay(500);
    test_disableMPPT();
    delay(500);
    test_interruptCharging();
    delay(500);
    test_enableCharging();
    delay(500);
}


void loop() {
    

}
