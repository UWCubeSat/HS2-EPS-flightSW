#include "BQ25756_reg.h"
#include "i2c.h"

class BatteryMonitor : public BQ25756{
    public:
        void report_status();
        void printVBAT_LOWV();
    private:
        int getVac();
        int getVbat();
        int getVfb();
        int getIac();
        int getIbat();
        int getIchg();
        chargingStatus getChargingStatus();
};
