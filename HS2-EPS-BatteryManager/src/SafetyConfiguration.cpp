// Safety configuration setting
#include "SafetyConfiguration.h"

// ── Watchdog ──────────────────────────────────────────────────────────────────

// Disable Watchdog timer control (sets TIME_CONT[5:4] to 00)
void BQ25756::SafetyConfig::disableWatchdogTimerControl() { clearBit(TIME_CONT, 0x30); }
bool BQ25756::SafetyConfig::WatchdogTimerControlDisabled() { return !readBit(TIME_CONT, 0x30); }


// ── MPPT ──────────────────────────────────────────────────────────────────────

// When MPPT is enabled, the device controls the ADC; writes to REG2A are ignored
void BQ25756::SafetyConfig::enableMPPT()   { setBit(MPPT_CONT,   0x01); }
void BQ25756::SafetyConfig::disableMPPT()  { clearBit(MPPT_CONT, 0x01); }
bool BQ25756::SafetyConfig::MPPTenabled()  { return readBit(MPPT_CONT, 0x01); }


// ── TS pin ────────────────────────────────────────────────────────────────────

// Applies to both forward charging and reverse discharging modes
void BQ25756::SafetyConfig::EnableTSPinFunctionControl()   { setBit(CHARGE_REGION_CONT, 0x01); }
bool BQ25756::SafetyConfig::TSPinFunctionControlEnabled()  { return readBit(CHARGE_REGION_CONT, 0x01); }


// ── INT flags (0 = normal, 1 = event occurred) ────────────────────────────────

bool BQ25756::SafetyConfig::MPPTINTFlagNormal() { return !readBit(CHARGER_FLAG_2, 0x01); }
bool BQ25756::SafetyConfig::TSFlagNormal()      { return !readBit(CHARGER_FLAG_2, 0x02); }
bool BQ25756::SafetyConfig::PGFlagNormal()      { return !readBit(CHARGER_FLAG_2, 0x80); }


// ── Charge enable ─────────────────────────────────────────────────────────────

bool BQ25756::SafetyConfig::chargeEnableControl() { return readBit(CHARGER_CONT,   0x01); }
void BQ25756::SafetyConfig::interruptCharging()   { clearBit(CHARGER_CONT, 0x01); }
void BQ25756::SafetyConfig::enableCharging()      { setBit(CHARGER_CONT,   0x01); }
