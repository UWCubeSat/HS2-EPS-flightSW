#include <Wire.h>
#include "BQ25756.h"
#include "test_HeatShutup.h"

int serial_putchar(char c, FILE* f)
{
    Serial.write(c);
    return c;
}