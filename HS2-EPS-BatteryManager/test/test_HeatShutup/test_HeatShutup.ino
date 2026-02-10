#include <Wire.h>
#include "BQ25756.h"
#include "test_HeatShutup.h"

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

    Wire.begin();
    Wire.beginTransmission(0x6B);
    
    Serial.println("Startig I2C....");
    delay(500);

    printf("current state\n");
    print_state();
    delay(500);

    printf("Test disable functions\n");
    
    test_JEITA_disable ();
    delay(500);
    test_TS_disable ();
    delay(500);

    printf("Test read disable functions\n");
    test_is_JEITAdisabled();
    delay(500);
    test_is_TSdisabled ();
    delay(500);

    printf("Test enable functions\n");
    test_JEITA_enable ();
    delay(500);
    test_TS_enable();
    delay(500);

    printf("Test TS ADC value\n");
    test_TS_ADC_PRCNT ();
    delay(500);

    printf("Test TS ADC value\n");
    test_readTS_STAT();
    delay(500);

    printf("Test TS T5 values\n");
    test_configure_TS_T5_Charging_Threshold(BQ25756::HeatShutup::T5_41p2);
    delay(500);

    test_configure_TS_T5_Charging_Threshold(BQ25756::HeatShutup::T5_37p7);
    delay(500);

    test_configure_TS_T5_Charging_Threshold(BQ25756::HeatShutup::T5_34p375);
    delay(500);

    test_configure_TS_T5_Charging_Threshold(BQ25756::HeatShutup::T5_31p25);
    delay(500);

    printf("Test TS T3 values\n");
    test_configure_TS_T3_Charging_Threshold(BQ25756::HeatShutup::T3_48p4);
    delay(500);

    test_configure_TS_T3_Charging_Threshold(BQ25756::HeatShutup::T3_44p8);
    delay(500);

    test_configure_TS_T3_Charging_Threshold(BQ25756::HeatShutup::T3_41p2);
    delay(500);

    test_configure_TS_T3_Charging_Threshold(BQ25756::HeatShutup::T3_37p7);
    delay(500);

    printf("Test TS T2 values\n");
    test_configure_TS_T2_Charging_Threshold(BQ25756::HeatShutup::T2_71p1);
    delay(500);

    test_configure_TS_T2_Charging_Threshold(BQ25756::HeatShutup::T2_68p4);
    delay(500);

    test_configure_TS_T2_Charging_Threshold(BQ25756::HeatShutup::T2_65p5);
    delay(500);

    test_configure_TS_T2_Charging_Threshold(BQ25756::HeatShutup::T2_62p4);
    delay(500);

    printf("Test TS T1 values\n");
    test_configure_TS_T1_Charging_Threshold(BQ25756::HeatShutup::T1_77p15);
    delay(500);
    test_configure_TS_T1_Charging_Threshold(BQ25756::HeatShutup::T1_75p32);
    delay(500);
    test_configure_TS_T1_Charging_Threshold(BQ25756::HeatShutup::T1_73p25);
    delay(500);
    test_configure_TS_T1_Charging_Threshold(BQ25756::HeatShutup::T1_71p1);
    delay(500);

    printf("Test TS level reset\n");
    test_reset_TS_lvl();
    delay(500);
}

void loop() {
    

}
