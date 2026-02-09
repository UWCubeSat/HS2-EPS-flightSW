#ifndef TEST_SAFETY_H
#define TEST_SAFETY_H

void print_state();
void test_disableWatchdogTimerControl();
void test_EnableTSPinFunctionControl();
void test_enableMPPT();
void test_disableMPPT();
void test_interruptCharging();
void test_enableCharging();

#endif