#ifndef BQ25756_H
#define BQ25756_H

#include "i2c.h"
#include "BQ25756_reg.h"


class BQ25756 {
    public:
        void printInitializationStatus();
        class ADCControl {
            public:
                void enableADC();
                bool ADCEnabled();
                bool getADCRate();
                void enableIAC_ADC();
                void enableIBAT_ADC();
                void enableVAC_ADC();
                void enableVBAT_ADC();
                void enableTS_ADC();
                void enableVFB_ADC();
                void enableAllADCControl();
        };
    ADCControl adc;    
};

#endif