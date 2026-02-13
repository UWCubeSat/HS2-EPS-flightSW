// Write test code here and call the function in .ino file to run the test
#include "BatteryMonitor.h"
#include "test_BatteryMonitor.h"
BQ25756 bq;

// This function should be called before getting battery status
void enableADCSequence() {
    bq.adc.enableAllADCControl();
    bq.adc.setADCContinuous();
    bq.adc.enableADC();
}

void reportStatus() {
    auto measurements = bq.bm->getMeasurements();
    printf("VAC: %d mV\n", measurements.vac);
    printf("VBAT: %d mV\n", measurements.vbat);
    printf("VFB: %d mV\n", measurements.vfb);
    printf("VRECHG: %d mV\n", measurements.vrechg);
    printf("VBAT_LOWV: %d mV\n", measurements.vbat_lowv);
    printf("ICHG: %d mA\n", measurements.ichg);
    printf("IAC: %d mA\n", measurements.iac);
    printf("IBAT: %d mA\n", measurements.ibat);

    auto status = bq.bm->getChargingStatus();
    printf("Charging Status: %s\n", bq.bm->toString(status));
    
}


void printVBAT_LOWV() {
        uint8_t data = read8bitRegister(PRECHARGE_TERM_CONT);
        uint8_t bit2_1Value = (data & 0x06) >> 1;
        printf("VBAT_LOW is");
        printf(bit2_1Value);
    }

    
