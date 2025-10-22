
#include <stdint.h>  // Include standard integer types for uint8_t, uint16_t, etc.
#include "BQ25756_ard.h"

// Register addresses
#define TS_CHARGING_THRESHOLD_REG           0x1B
#define TS_CHARGING_REGION_BEHAVIOR_REG     0x1C
#define TS_STAT_REG                         0x22
#define TS_ADC_REG                          0x2C
#define IAC_ADC_REG                         0x2D
#define IAC_ADC_HIGH_REG                    0x2E

// Function prototypes for reading and writing registers
uint8_t read8BitRegister(uint8_t regAddress);
void writeRegister(uint8_t regAddress, uint8_t value);

// TS_T1 temperature option
typedef enum {
    TS_T1_N10C = 0x00,  // -10°C
    TS_T1_N5C = 0x01,  // -5°C
    TS_T1_0C = 0x02,  // 0°C
    TS_T1_5C = 0x3   // 5°C
} TS_T1;

// TS_T2 temperature option
typedef enum {
    TS_T2_5C  = 0x00,
    TS_T2_10C = 0x01,
    TS_T2_15C = 0x02,
    TS_T2_20C = 0x03
} TS_T2;

// TS_T3 temperature option
typedef enum {
    TS_T3_40C = 0x00,  // 40°C
    TS_T3_45C = 0x01,  // 45°C
    TS_T3_50C = 0x02,  // 50°C
    TS_T3_55C = 0x03   // 55°C
} TS_T3;

// TS_T5 temperature option
typedef enum {
    TS_T5_50C = 0x00,  // 50°C
    TS_T5_55C = 0x01,  // 55°C
    TS_T5_60C = 0x02,  // 60°C
    TS_T5_65C = 0x03   // 65°C
} TS_T5;

// Function to configure TS Charging Threshold Control
uint8_t configure_TS_Charging_Threshold_Control(TS_T1 ts_t1, TS_T2 ts_t2, TS_T3 ts_t3, TS_T5 ts_t5);
// Enum for JEITA voltage setting (JEITA_VSET)
typedef enum {
    JEITA_VSET_CHARGE_SUSPEND = 0x00,   // Charge Suspend
    JEITA_VSET_94_3_PERCENT = 0x01,     // 94.3% of VFB_REG
    JEITA_VSET_97_6_PERCENT = 0x02,     // 97.6% of VFB_REG
    JEITA_VSET_100_PERCENT = 0x03      // 100% of VFB_REG
} JEITA_VSET;

// Enum for JEITA Warm current setting (JEITA_ISETH)
typedef enum {
    JEITA_ISETH_40_PERCENT = 0x00,   // 40% of ICHG_REG
    JEITA_ISETH_100_PERCENT = 0x01   // 100% of ICHG_REG
} JEITA_ISETH;

// Enum for JEITA Cool current setting (JEITA_ISETC)
typedef enum {
    JEITA_ISETC_CHARGE_SUSPEND = 0x00,  // Charge Suspend
    JEITA_ISETC_20_PERCENT = 0x01,      // 20% of ICHG_REG
    JEITA_ISETC_40_PERCENT = 0x02,      // 40% of ICHG_REG
    JEITA_ISETC_100_PERCENT = 0x03     // 100% of ICHG_REG
} JEITA_ISETC;

// Enum for enabling JEITA (EN_JEITA)
typedef enum {
    EN_JEITA_DISABLED = 0x00,   // Disabled (only COLD/HOT control)
    EN_JEITA_ENABLED = 0x01     // Enabled (COLD/COOL/WARM/HOT control)
} EN_JEITA;

// Enum for enabling TS pin function (EN_TS)
typedef enum {
    EN_TS_DISABLED = 0x00,  // Disabled (ignore TS pin)
    EN_TS_ENABLED = 0x01    // Enabled
} EN_TS;

// Function to configure TS Charging Region Behavior Control
uint8_t configure_TS_Charging_Region(
    JEITA_VSET vset, JEITA_ISETH iseth, JEITA_ISETC isetc, EN_JEITA en_jeita, EN_TS en_ts
);

// Function to get TS status
uint8_t getTS_STAT();

// Function to enable TS_ADC
void TS_ADC_enable();


