#ifndef BQ25756_H
#define BQ25756_H

#include "i2c.h"
#include "BQ25756_reg.h"


class BQ25756 {
    public:
        class ADCControl {
            public:
                // bool functions
                bool isADCEnabled();
                bool isADCRateOneshot();
                bool isIBAT_ADCDisabled();
                bool isIAC_ADCDisabled();
                bool isVAC_ADCDisabled();
                bool isVBAT_ADCDisabled(); 
                bool isTS_ADCDisabled();
                bool isVFB_ADCDisabled();
                
                // Enable ADC functions
                void enableADC();
                void enableIAC_ADC();
                void enableIBAT_ADC();
                void enableVAC_ADC();
                void enableVBAT_ADC();
                void enableTS_ADC();
                void enableVFB_ADC();
                void enableAllADCControl();

                // Disable ADC functions
                void disableADC();
                void disableIAC_ADC();
                void disableIBAT_ADC();
                void disableVAC_ADC();
                void disableVBAT_ADC();
                void disableTS_ADC();
                void disableVFB_ADC();
        };
        class BatteryMonitor;
        class HeatShutup;
    ADCControl adc;    
};

#endif