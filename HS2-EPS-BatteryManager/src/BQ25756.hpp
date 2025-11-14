#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

class BQ25756 {
    public:
        void get_status();
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
        }
}