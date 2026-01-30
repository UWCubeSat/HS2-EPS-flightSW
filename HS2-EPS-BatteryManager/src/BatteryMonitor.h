#include "BQ25756_reg.h"
#include "i2c.h"
#include <map>
#include <string>

class BatteryMonitor : public BQ25756{
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
