#ifndef TEST_HEATSHUTUP_H
#define TEST_HEATSHUTUP_H
#include "BQ25756.h"
#include "HeatShutup.h"

void print_state();
void test_JEITA_enable ();
void test_TS_enable();
void test_JEITA_disable ();
void test_is_JEITAdisabled();
void test_TS_disable ();
void test_is_TSdisabled ();
void test_TS_ADC_PRCNT ();
void test_readTSVoltagePercent();
void test_readTS_STAT();
void test_configure_TS_T5_Charging_Threshold (BQ25756::HeatShutup::TS_T5_prcnt lvl);
void test_configure_TS_T3_Charging_Threshold (BQ25756::HeatShutup::TS_T3_prcnt lvl);
void test_configure_TS_T2_Charging_Threshold (BQ25756::HeatShutup::TS_T2_prcnt lvl);
void test_configure_TS_T1_Charging_Threshold (BQ25756::HeatShutup::TS_T1_prcnt lvl);
void test_reset_TS_lvl();


#endif