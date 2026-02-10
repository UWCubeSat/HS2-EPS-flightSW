// Write test code here and call the function in .ino file to run the test
// We could add delay in each test function
#include "SafetyConfiguration.h"
#include <stdio.h>
BQ25756 bq;

// ADC control state for BQ25756
struct SafetyConfigState {
  bool watchdog_timer_disabled;
  bool MPPT_enabled;
  bool TSPin_funcCont_disabled;
  bool MPPTIntFlag_normal;
  bool TSFlag_normal;
  bool PSFlag_normal;
  bool charge_enabled;
};

// Get ADC control state by reading registers
static SafetyConfigState readSafetyConfigState()
{
  SafetyConfigState s{};
  if (bq.sfconfig) {
    s.watchdog_timer_disabled = bq.sfconfig->WatchdogTimerControlDisabled();
    s.MPPT_enabled= bq.sfconfig->MPPTenabled();
    s.TSPin_funcCont_disabled = bq.sfconfig->TSPinFunctionControlEnabled();
    s.MPPTIntFlag_normal = bq.sfconfig->MPPTINTFlagNormal();
    s.TSFlag_normal = bq.sfconfig->TSFlagNormal();
    s.PSFlag_normal = bq.sfconfig->PGFlagNormal();
    s.charge_enabled = bq.sfconfig->chargeEnableControl();
  }
  return s;
}

// Print Safety Configuration state showing normal / enabled status
static void printSafetyConfigState(const SafetyConfigState& s)
{
  printf(
    "SAFETY | WDT:%s MPPT:%s TS_FUNC:%s | MPPT_INT:%s TS:%s PG:%s\n",
    s.watchdog_timer_disabled ? "DIS" : "EN",
    s.MPPT_enabled            ? "EN"  : "DIS",
    s.TSPin_funcCont_disabled ? "DIS" : "EN",
    s.MPPTIntFlag_normal      ? "OK"  : "FAULT",
    s.TSFlag_normal           ? "OK"  : "FAULT",
    s.PSFlag_normal           ? "OK"  : "FAULT",
    s.charge_enabled          ? "EN"  : "DIS"
  );
}


// Print function
void print_state()
{
    printSafetyConfigState(readSafetyConfigState());
}

// Test BQ25756::SafetyConfig::disableWatchdogTimerControl()
void test_disableWatchdogTimerControl()
{
    printf("\n[TEST] Watchdog Timer Control\n");
    printf("Before: ");
    printSafetyConfigState(readSafetyConfigState());
    bq.sfconfig->disableWatchdogTimerControl();
    printf("After : ");
    printSafetyConfigState(readSafetyConfigState());
}

// Test BQ25756::SafetyConfig::EnableTSPinFunctionControl()
void test_EnableTSPinFunctionControl()
{
    printf("\n[TEST] Enable TSPin Function Control\n");
    printf("Before: ");
    printSafetyConfigState(readSafetyConfigState());
    bq.sfconfig->EnableTSPinFunctionControl();
    printf("After : ");
    printSafetyConfigState(readSafetyConfigState());
}

// Test BQ25756::SafetyConfig::enablMPPT()
void test_enableMPPT()
{
    printf("\n[TEST] Enable MPPT\n");
    printf("Before: ");
    printSafetyConfigState(readSafetyConfigState());
    bq.sfconfig->enableMPPT();
    printf("After : ");
    printSafetyConfigState(readSafetyConfigState());
}

// Test BQ25756::SafetyConfig::disaablMPPT()
void test_disableMPPT()
{
    printf("\n[TEST] Disable MPPT\n");
    printf("Before: ");
    printSafetyConfigState(readSafetyConfigState());
    bq.sfconfig->disableMPPT();
    printf("After : ");
    printSafetyConfigState(readSafetyConfigState());
}

// Test BQ25756::SafetyConfig::interruptCharging()
void test_interruptCharging()
{
    printf("\n[TEST] Interrupt charging\n");
    printf("Check the battery status.....");
    bq.sfconfig->interruptCharging();
    printf("After : ");
    printSafetyConfigState(readSafetyConfigState());
}

void test_enableCharging()
{
    printf("\n[TEST] Interrupt charging\n");
    printf("Check the battery status.....");
    bq.sfconfig->enableCharging();
    printf("After : ");
    printSafetyConfigState(readSafetyConfigState());
}