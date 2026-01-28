// Write test code here and call the function in .ino file to run the test
// We could add delay in each test function
#include "BQ25756.h"
BQ25756 bq;

// ADC control state for BQ25756
struct ADCControlState {
  bool adc_enabled;
  bool adc_rate_oneshot;
  bool ibat_disabled;
  bool iac_disabled;
  bool vac_disabled;
  bool vbat_disabled;
  bool ts_disabled;
  bool vfb_disabled;
};

// Get ADC control state by reading registers
static ADCControlState readADCControlState()
{
  ADCControlState s{};

  s.adc_enabled      = bq.adc.isADCEnabled();
  s.adc_rate_oneshot = bq.adc.isADCRateOneshot();
  s.ibat_disabled = bq.adc.isIBAT_ADCDisabled();
  s.iac_disabled  = bq.adc.isIAC_ADCDisabled();
  s.vac_disabled  = bq.adc.isVAC_ADCDisabled();
  s.vbat_disabled = bq.adc.isVBAT_ADCDisabled();
  s.ts_disabled   = bq.adc.isTS_ADCDisabled();
  s.vfb_disabled  = bq.adc.isVFB_ADCDisabled();
  return s;
}

// Print ADC control state showing enabled or disabled
static void printADCControlState(const ADCControlState& s)
{
  printf(
    "ADC:%s Rate:%s | IBAT:%s IAC:%s VAC:%s VBAT:%s TS:%s VFB:%s\n",
    s.adc_enabled ? "EN" : "DIS",
    s.adc_rate_oneshot ? "1SHOT" : "CONT",
    s.ibat_disabled ? "DIS" : "EN",
    s.iac_disabled  ? "DIS" : "EN",
    s.vac_disabled  ? "DIS" : "EN",
    s.vbat_disabled ? "DIS" : "EN",
    s.ts_disabled   ? "DIS" : "EN",
    s.vfb_disabled  ? "DIS" : "EN"
);
}

// Print function
void print_state()
{
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableADC()
void test_ADC_enable()
{
  printf("\n[TEST] ADC Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableIAC_ADC()
void test_enableIAC_ADC()
{
  printf("\n[TEST] IAC ADC Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableIAC_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableIBAT_ADC()
void test_enableIBAT_ADC()
{
  printf("\n[TEST] IBAT ADC Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableIBAT_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableVAC_ADC()
void test_enableVAC_ADC()
{
  printf("\n[TEST] VAC ADC Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableVAC_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableVBAT_ADC()
void test_enableVBAT_ADC()
{
  printf("\n[TEST] VBAT ADC Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableVBAT_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableTS_ADC()
void test_enableTS_ADC()
{
  printf("\n[TEST] TS ADC Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableTS_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableVFB_ADC()
void test_enableVFB_ADC()
{
  printf("\n[TEST] VFB ADC Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableVFB_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:enableAllADCControl()
void test_enableAllADCControl()
{
  printf("\n[TEST] All ADC Control Enable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.enableAllADCControl();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:disableADC()
void test_ADC_disable()
{
  printf("\n[TEST] ADC Disable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.disableADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:disableIAC_ADC()
void test_disableIAC_ADC()
{
  printf("\n[TEST] IAC ADC Disable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.disableIAC_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:disableIBAT_ADC()
void test_disableIBAT_ADC()
{
  printf("\n[TEST] IBAT ADC Disable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.disableIBAT_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:disableVAC_ADC()
void test_disableVAC_ADC()
{
  printf("\n[TEST] VAC ADC Disable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.disableVAC_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:disableVBAT_ADC()
void test_disableVBAT_ADC()
{
  printf("\n[TEST] VBAT ADC Disable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.disableVBAT_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:disableTS_ADC()
void test_disableTS_ADC()
{
  printf("\n[TEST] TS ADC Disable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.disableTS_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}

// Test BQ25756:ADCControl:disableVFB_ADC()
void test_disableVFB_ADC()
{
  printf("\n[TEST] VFB ADC Disable\n");

  printf("Before: ");
  printADCControlState(readADCControlState());

  bq.adc.disableVFB_ADC();

  printf("After : ");
  printADCControlState(readADCControlState());
}
