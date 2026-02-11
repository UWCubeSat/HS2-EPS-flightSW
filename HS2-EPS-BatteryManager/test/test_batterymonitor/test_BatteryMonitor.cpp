// Write test code here and call the function in .ino file to run the test
#include "BatteryMonitor.h"
#include "test_BatteryMonitor.h"

void reportStatus() {
            int properties[8];
            bq.batteryMonitor->getProperties(properties);
            
            for (size_t i = 0; i < properties.size(); i++) {
                Serial.print(properties[i].key.c_str());
                Serial.print(": ");
                Serial.println(properties[i].value);
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

    
