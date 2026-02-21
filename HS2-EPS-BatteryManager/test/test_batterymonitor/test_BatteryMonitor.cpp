#include "BatteryMonitor.h"
#include "test_BatteryMonitor.h"
#include "BQ25756.h"

extern BQ25756 bq;   // bq is defined in the .ino file

void reportStatus() {
    int properties[8];
    bq.batteryMonitor->getProperties(properties);

    const char* names[8] = {
        "VAC", "VBAT", "VFB", "VRECHG",
        "VBAT_LOWV", "ICHG", "IAC", "IBAT"
    };

    for (size_t i = 0; i < 8; i++) {
        Serial.print(names[i]);
        Serial.print(": ");
        Serial.println(properties[i]);
    }

    auto status = bq.batteryMonitor->getChargingStatus();
    Serial.print("Charging Status: ");
    Serial.println(bq.batteryMonitor->toString(status));
}

void printVBAT_LOWV() {
    Serial.print("VBAT_LOWV: ");
    Serial.println(bq.batteryMonitor->readVbat_lowv());
}

void enableCharging() {
    uint8_t currValue = read8bitRegister(CHARGER_CONT);
    uint8_t newValue = currValue | (0x01);
    writeRegister(CHARGER_CONT, newValue);
}