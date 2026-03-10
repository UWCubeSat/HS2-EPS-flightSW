// Write test code here and call the function in .ino file to run the test
// We could add delay in each test function
#include "FaultStatus.h"
#include <stdio.h>
BQ25756 bq;

// Fault status state for BQ25756
struct FaultStatusState {
  bool Input_under_voltage_fault;
  bool Input_over_voltage_fault;
  bool battery_over_current_fault;
  bool battery_over_voltage_fault;
  bool thermal_shutdown_fault;
  bool charge_safety_timer_fault;
  bool DRV_SUP_pin_fault;
};

// Read the current falut status into the output struct.
// Return true if reading succeed.
// Return false if the interface is not initialized.
static bool readSafetyConfigState(FaultStatusState &s)
{
  s = {};
  if (!bq.fs) {
    printf("EROOR: Fault Status is not initialized.\n");
    return false;
  }
  
  s.Input_under_voltage_fault = bq.fs->InputUnderVoltageFault();
  s.Input_over_voltage_fault= bq.fs->InputOverVoltageFault();
  s.battery_over_current_fault = bq.fs->BatteryOverCurrentFault();
  s.battery_over_voltage_fault = bq.fs->BatteryOverVoltageFault();
  s.thermal_shutdown_fault = bq.fs->ThermalShutdownFault();
  s.charge_safety_timer_fault = bq.fs->ChargeSafetyTimerFault();
  s.DRV_SUP_pin_fault = bq.fs->DRV_SUPPinFault();
  
  return true;
}

// Print Safety Configuration state showing normal / enabled status
static void printFaultStatusState(const FaultStatusState& s)
{
  printf(
    "FAULT_STATUS | VAC_UV_STAT:%s VAC_OV_STAT:%s IBAT_OCP_STAT:%s VBAT_OV_STAT:%s TSHUT_STAT:%s CHG_TMR_STAT:%s DRV_OKZ_STAT: %s\n",
    s.Input_under_voltage_fault   ? "Fault" : "Normal",
    s.Input_over_voltage_fault    ? "Fault" : "Normal",
    s.battery_over_current_fault  ? "Fault" : "Normal",
    s.battery_over_voltage_fault  ? "Fault" : "Normal",
    s.thermal_shutdown_fault      ? "Fault" : "Normal",
    s.charge_safety_timer_fault   ? "Fault" : "Normal",
    s.DRV_SUP_pin_fault           ? "Fault" : "Normal"
  );
}


// Print function
void print_state()
{
    FaultStatusState state{};
    if (!readSafetyConfigState(state)) {
      return;
    }
    printFaultStatusState(state);
}
