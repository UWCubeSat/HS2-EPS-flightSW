#include "HeatShutup.h"
#include <Wire.h>
#include "BQ25756_reg.h"

BQ25756 bq1;

// ── Private helper ────────────────────────────────────────────────────────────

// Clears the field selected by clearMask in CHARGE_THRESH_CONT, then ORs in setValue.
void BQ25756::HeatShutup::setThresholdField(uint8_t clearMask, uint8_t setValue)
{
    uint8_t curr = read8bitRegister(CHARGE_THRESH_CONT);
    writeRegister(CHARGE_THRESH_CONT, (curr & clearMask) | setValue);
}


// ── TS status & voltage ───────────────────────────────────────────────────────

// Obtain the status of the Thermal Shutdown.
// Returns TS_LVL based on the JEITA temperature zone.
// This IC jumps T4, so we return TS_HOT for case 4 (effectively T5).
BQ25756::HeatShutup::TS_LVL BQ25756::HeatShutup::readTS_STAT()
{
    uint8_t field = (read8bitRegister(CHARGER_STATUS_2) >> 4) & 0x07;
    switch (field)
    {
        case 0:  return TS_NORMAL;
        case 1:  return TS_WARM;
        case 2:  return TS_COOL;
        case 3:  return TS_COLD;
        case 4:  return TS_HOT;
        default: return TS_INVALID;
    }
}

// Returns TS pin voltage as a percentage of REGN (0–100%).
float BQ25756::HeatShutup::readTSVoltagePercent()
{
    bq1.adc.enableADCReadingForOneshot();
    uint16_t tsADC = read16BitRegister(TS_ADC) & 0x03FF;
    return (tsADC / 1024.0f) * 100;
}


// ── JEITA / TS enable ─────────────────────────────────────────────────────────

void BQ25756::HeatShutup::JEITA_enable()  { setBit(CHARGE_REGION_CONT,   0x02); }
void BQ25756::HeatShutup::JEITA_disable() { clearBit(CHARGE_REGION_CONT, 0x02); }
void BQ25756::HeatShutup::TS_enable()     { setBit(CHARGE_REGION_CONT,   0x01); }
void BQ25756::HeatShutup::TS_disable()    { clearBit(CHARGE_REGION_CONT, 0x01); }

bool BQ25756::HeatShutup::isJEITAdisabled() { return !readBit(CHARGE_REGION_CONT, 0x02); }
bool BQ25756::HeatShutup::isTSdisabled()    { return !readBit(CHARGE_REGION_CONT, 0x01); }


// ── Charging threshold configuration ─────────────────────────────────────────
//
// CHARGE_THRESH_CONT bit layout:
//   [7:6] T5  [5:4] T3  [3:2] T2  [1:0] T1
//
// The enum values (0–3) map directly to the 2-bit field value for each zone,
// so we just shift the enum into position.

void BQ25756::HeatShutup::configure_TS_T5_Charging_Threshold(TS_T5_prcnt userInput)
{
    setThresholdField(0x3F, userInput << 6);
}

void BQ25756::HeatShutup::configure_TS_T3_Charging_Threshold(TS_T3_prcnt userInput)
{
    setThresholdField(0xCF, userInput << 4);
}

void BQ25756::HeatShutup::configure_TS_T2_Charging_Threshold(TS_T2_prcnt userInput)
{
    setThresholdField(0xF3, userInput << 2);
}

void BQ25756::HeatShutup::configure_TS_T1_Charging_Threshold(TS_T1_prcnt userInput)
{
    setThresholdField(0xFC, userInput);
}

void BQ25756::HeatShutup::reset_TS_lvl()
{
    configure_TS_T5_Charging_Threshold(T5_34p375);
    configure_TS_T3_Charging_Threshold(T3_44p8);
    configure_TS_T2_Charging_Threshold(T2_68p4);
    configure_TS_T1_Charging_Threshold(T1_73p25);
}
