#ifndef HEAT_SHUTUP_H
#define HEAT_SHUTUP_H

#include "BQ25756_reg.h"
#include "i2c.h"

class HeatShutup: public BQ25756{
    public:
        TS_LVL readTS_STAT();
        float TS_ADC();
        void JEITA_enable();
        void JEITA_disable();
        void TS_enable();
        void TS_disable();
        void configure_TS_T5_Charging_Threshold (TS_T5_prcnt userInput);
        void configure_TS_T3_Charging_Threshold (TS_T3_prcnt userInput);
        void configure_TS_T2_Charging_Threshold (TS_T2_prcnt userInput);
        void configure_TS_T1_Charging_Threshold (TS_T1_prcnt userInput);
        

};

#endif