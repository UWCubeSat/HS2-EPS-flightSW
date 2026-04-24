// Outer class for BQ25756 should be included here
#include "BQ25756.h"
#include "BatteryMonitor.h"
#include "HeatShutup.h"
#include "SafetyConfiguration.h"
#include "FaultStatus.h"


BQ25756::BQ25756()
{
    bm = new BatteryMonitor();
    fs = new FaultStatus();
    hs = new HeatShutup();
    sfconfig = new SafetyConfig();
}

/**
 *  @brief Reset register
 *
 *  Reset all the registers to the default value
 *  by writing REG_RST to 1.
 *  REG_RST goes back to 0 automatically after writing to 1.
 */
void BQ25756::resetRegister()
{
    setBit(POW_PATH_REV_CONT, 0x80);
}


// ── ADC main control ──────────────────────────────────────────────────────────

bool BQ25756::ADCControl::isADCEnabled()      { return readBit(ADC_CONT, 0x80); }
bool BQ25756::ADCControl::isADCRateOneshot()  { return readBit(ADC_CONT, 0x40); }

void BQ25756::ADCControl::enableADC()         { setBit(ADC_CONT,   0x80); }
void BQ25756::ADCControl::disableADC()        { clearBit(ADC_CONT, 0x80); }
// ADC rate must be continuous; otherwise ADC_EN is always cleared after each conversion
void BQ25756::ADCControl::setADCContinuous()  { clearBit(ADC_CONT, 0x40); }

// Enable all ADC channels at once by clearing the entire disable register
void BQ25756::ADCControl::enableAllADCControl() { writeRegister(ADC_CHANNEL_CONT, 0x00); }


// ── ADC channel control (bit=1 disables the channel) ─────────────────────────
//
// ADC_CHANNEL_CONT bit positions:
//   IAC=7  IBAT=6  VAC=5  VBAT=4  TS=2  VFB=1

bool BQ25756::ADCControl::isIAC_ADCDisabled()  { return readBit(ADC_CHANNEL_CONT, 0x80); }
bool BQ25756::ADCControl::isIBAT_ADCDisabled() { return readBit(ADC_CHANNEL_CONT, 0x40); }
bool BQ25756::ADCControl::isVAC_ADCDisabled()  { return readBit(ADC_CHANNEL_CONT, 0x20); }
bool BQ25756::ADCControl::isVBAT_ADCDisabled() { return readBit(ADC_CHANNEL_CONT, 0x10); }
bool BQ25756::ADCControl::isTS_ADCDisabled()   { return readBit(ADC_CHANNEL_CONT, 0x04); }
bool BQ25756::ADCControl::isVFB_ADCDisabled()  { return readBit(ADC_CHANNEL_CONT, 0x02); }

void BQ25756::ADCControl::enableIAC_ADC()    { clearBit(ADC_CHANNEL_CONT, 0x80); }
void BQ25756::ADCControl::enableIBAT_ADC()   { clearBit(ADC_CHANNEL_CONT, 0x40); }
void BQ25756::ADCControl::enableVAC_ADC()    { clearBit(ADC_CHANNEL_CONT, 0x20); }
void BQ25756::ADCControl::enableVBAT_ADC()   { clearBit(ADC_CHANNEL_CONT, 0x10); }
void BQ25756::ADCControl::enableTS_ADC()     { clearBit(ADC_CHANNEL_CONT, 0x04); }
void BQ25756::ADCControl::enableVFB_ADC()    { clearBit(ADC_CHANNEL_CONT, 0x02); }

void BQ25756::ADCControl::disableIAC_ADC()   { setBit(ADC_CHANNEL_CONT, 0x80); }
void BQ25756::ADCControl::disableIBAT_ADC()  { setBit(ADC_CHANNEL_CONT, 0x40); }
void BQ25756::ADCControl::disableVAC_ADC()   { setBit(ADC_CHANNEL_CONT, 0x20); }
void BQ25756::ADCControl::disableVBAT_ADC()  { setBit(ADC_CHANNEL_CONT, 0x10); }
void BQ25756::ADCControl::disableTS_ADC()    { setBit(ADC_CHANNEL_CONT, 0x04); }
void BQ25756::ADCControl::disableVFB_ADC()   { setBit(ADC_CHANNEL_CONT, 0x02); }
