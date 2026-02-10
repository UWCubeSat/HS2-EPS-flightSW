// Write test code here and call the function in .ino file to run the test
#include "BatteryMonitor.h"

void reportStatus() {
            
            for (const auto& [key, value] : getProperties()) {
                Serial.print(key.c_str());
                Serial.print(": ");
                Serial.println(value);
            }

            ChargingStatus status = getChargingStatus();
            Serial.print("Charging Status: ");
            Serial.println(BQ25756::BatteryMonitor::toString(status));
            
        }

        
void printVBAT_LOWV() {
        uint8_t data = read8BitRegister(PRECHARGE_TERM_CONT);
        uint8_t bit2_1Value = (data & 0x06) >> 1;
        Serial.print("VBAT_LOW is");
        Serial.println(bit2_1Value);
    }

    
