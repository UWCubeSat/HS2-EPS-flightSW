#include "BatteryMonitor.h"
#include "test_BatteryMonitor.h"
BQ25756 b2;

// This function should be called before getting battery status
void enableADCSequence() {
    b2.adc.enableAllADCControl();
    b2.adc.setADCContinuous();
    b2.adc.enableADC();
    b2.adc.enableADCReadingForOneshot(); 
}

// Report all ADC measurements and charging status
void reportStatus() {
    auto measurements = b2.bm->getMeasurements();
    printf("VAC: %d mV\n", measurements.vac);
    printf("VBAT: %d mV\n", measurements.vbat);
    printf("VFB: %d mV\n", measurements.vfb);
    printf("VRECHG: %d mV\n", measurements.vrechg);
    printf("VBAT_LOWV: %d mV\n", measurements.vbat_lowv);
    printf("ICHG: %d mA\n", measurements.ichg);
    printf("IAC: %d mA\n", measurements.iac);
    printf("IBAT: %d mA\n", measurements.ibat);

    auto status = b2.bm->getChargingStatus();
    printf("Charging Status: %s\n", b2.bm->toString(status));
    
}


void printVBAT_LOWV() {
        uint8_t data = read8bitRegister(PRECHARGE_TERM_CONT);
        uint8_t bit2_1Value = (data & 0x06) >> 1;
        printf("VBAT_LOW is ");
        printf("%d \n", bit2_1Value); //fix this
    }

    
