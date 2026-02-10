#ifndef HEAT_SHUTUP_H
#define HEAT_SHUTUP_H

#include "BQ25756_reg.h"
#include "i2c.h"
#include "BQ25756.h"
class BQ25756::HeatShutup{
    public:
        enum TS_LVL {
            TS_NORMAL,
            TS_WARM,
            TS_COOL,
            TS_COLD,
            TS_HOT,
            TS_INVALID
        };

        enum TS_T5_prcnt { 
            T5_41p2,
            T5_37p7,
            T5_34p375,
            T5_31p25
        };

        enum TS_T3_prcnt {
            T3_48p4,   
            T3_44p8,   
            T3_41p2,   
            T3_37p7 
        };

        enum TS_T2_prcnt { 
            T2_71p1,  
            T2_68p4,   
            T2_65p5,   
            T2_62p4 
        };
        
        enum TS_T1_prcnt { 
            T1_77p15,  
            T1_75p32,  
            T1_73p25,  
            T1_71p1 
        };

        TS_LVL readTS_STAT();
        float readTSVoltagePercent();
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