#include "BQ25756_reg.h"
#include "i2c.h"
#include <map>
#include <string>

class BQ25756::BatteryMonitor{
    public:
        const char* toString(ChargingStatus status);
        std::map<std::string, int> getProperties();
    private:
        int getVac();
        int getVbat();
        int getVfb();
        int getIac();
        int getIbat();
        int getIchg();
        int readVrechg();
        int readVbat_lowv();
        int readIchg();
        ChargingStatus getChargingStatus();
};

enum class BQ25756::BatteryMonitor::ChargingStatus : uint8_t {
    NOT_CHARGING = 0x00,
    TRICKLE_CHARGE = 0x01,
    PRE_CHARGE = 0x02,
    FAST_CHARGE = 0x03,
    TAPER_CHARGE = 0x04,
    RESERVED = 0x05,
    TOP_OFF_TIMER_ACTIVE = 0x06,
    CHARGE_TERMINATION_DONE = 0x07
};
