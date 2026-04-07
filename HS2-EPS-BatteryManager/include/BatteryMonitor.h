#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include "BQ25756.h"
#include "BQ25756_reg.h"
#include "i2c.h"
#include <stdint.h>

// This defines the nested class that was forward-declared in BQ25756.h
class BQ25756::BatteryMonitor {
public:
    struct BatteryMeasurements {
    int vac;
    int vbat;
    int vfb;
    int vrechg;
    int vbat_lowv;
    int ichg;
    int iac;
    int ibat;
    };


    // Define the enum INSIDE the class
    enum class ChargingStatus : uint8_t {
        NOT_CHARGING = 0x00,
        TRICKLE_CHARGE = 0x01,
        PRE_CHARGE = 0x02,
        FAST_CHARGE = 0x03,
        TAPER_CHARGE = 0x04,
        RESERVED = 0x05,
        TOP_OFF_TIMER_ACTIVE = 0x06,
        CHARGE_TERMINATION_DONE = 0x07
    };

    const char* toString(ChargingStatus status);
    BatteryMeasurements getMeasurements();
    ChargingStatus getChargingStatus();

private:
    int getVac();
    int getVbat();
    int getVfbReg();
    int getVfb();
    int getIac();
    int getIbat();
    int getIchg();
    int readVrechg();
    int readVbat_lowv();
    int readIchg();
};

#endif
