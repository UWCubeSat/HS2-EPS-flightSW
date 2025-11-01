// This control TS Charging Threshold

#include "BQ25756_ard.h"
#include "TS_charge_control.h"

#define TS_CHARGING_THRESHOLD_REG 0x1B
#define TS_CHARGING_REGION_BEHAVIOR_REG 0x1C

// Design: make function for each register to make codes organized

// REG0x1B_TS_Charging_Threshold_Control Register



// Configure TS_Charging_Threshold_Control
uint8_t configure_TS_Charging_Threshold_Control (TS_T1 ts_t1, TS_T2 ts_t2, TS_T3 ts_t3, TS_T5 ts_t5) {
    // Read register 
    uint8_t current_value = read8BitRegister(TS_CHARGING_THRESHOLD_REG);
    current_value &= ~(0x03);      // Clear TS_T1 (bits 0 and 1)
    current_value &= ~(0x03 << 2); // Clear TS_T2 (bits 2 and 3)
    current_value &= ~(0x03 << 4); // Clear TS_T3 (bits 4 and 5)
    current_value &= ~(0x03 << 6); // Clear TS_T5 (bits 6 and 7)

    current_value |= (ts_t1 << 0);  // Set TS_T1 in bits 0 and 1
    current_value |= (ts_t2 << 2);  // Set TS_T2 in bits 2 and 3
    current_value |= (ts_t3 << 4);  // Set TS_T3 in bits 4 and 5
    current_value |= (ts_t5 << 6);  // Set TS_T5 in bits 6 and 7

    writeRegister(0x1B, current_value);

    return read8BitRegister(TS_CHARGING_THRESHOLD_REG);
}


// REG0x1C TS_Charging_Region_Behavior_Control



// Configure TS_Charging_Region_Behavior
uint8_t configure_TS_Charging_Region(
    JEITA_VSET vset, JEITA_ISETH iseth, JEITA_ISETC isetc, EN_JEITA en_jeita, EN_TS en_ts) {
    // Read the current value of the register (mockup for simplicity)
    uint8_t current_value = read8BitRegister(TS_CHARGING_REGION_BEHAVIOR_REG);  

    // Clear the bits corresponding to the fields
    current_value &= ~(0x03 << 5); // Clear JEITA_VSET (bits 6:5)
    current_value &= ~(0x01 << 4); // Clear JEITA_ISETH (bit 4)
    current_value &= ~(0x03 << 2); // Clear JEITA_ISETC (bits 3:2)
    current_value &= ~(0x01 << 1); // Clear EN_JEITA (bit 1)
    current_value &= ~(0x01 << 0); // Clear EN_TS (bit 0)

    // Set the new values by shifting them into the corresponding positions
    current_value |= (vset << 5);    // Set JEITA_VSET (bits 6:5)
    current_value |= (iseth << 4);   // Set JEITA_ISETH (bit 4)
    current_value |= (isetc << 2);   // Set JEITA_ISETC (bits 3:2)
    current_value |= (en_jeita << 1); // Set EN_JEITA (bit 1)
    current_value |= (en_ts << 0);    // Set EN_TS (bit 0)

    // Write the updated value back to the register
    writeRegister(TS_CHARGING_REGION_BEHAVIOR_REG, current_value);

    // Return the updated 8-bit register value
    return read8BitRegister(TS_CHARGING_REGION_BEHAVIOR_REG);
}

// Get TS_STAT
// TS (Battery NTC) status:
// 000b = Normal
// 001b = TS Warm
// 010b = TS Cool
// 011b = TS Cold
// 100b = TS Hot
uint8_t getTS_STAT(){
    uint8_t readVal= read8BitRegister(0x22);
    readVal &= (0x07 << 4);
    readVal = readVal >> 4;

    return readVal;
}

// Enable TS_ADC
void TS_ADC_enable(){
    uint8_t register_value= read8BitRegister(0x2C);
    uint8_t mask = ~(0x04);
    register_value &= mask;

    writeRegister(0x2C, register_value);
}

