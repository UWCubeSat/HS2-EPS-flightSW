// define all the register here

#define I2C_BUS_ADDR 0x6B

// Define the Register address which gives you 16 bits data by riting the register

#define CHARGE_VOLT_LIM         0x00    // Charge_Voltage_Limit 
#define CHARGE_CURR_LIM         0x02    // Charge_Current_Limit 
#define IMP_CURR_DPM_LIM        0x06    // Input_Current_DPM_Limit 
#define IMP_VOLT_DPM_LIM        0x08    // Input_Voltage_DPM_Limit 
#define REV_IMP_CURR_LIM        0x0A    // Reverse_Mode_Input_Current_Limit
#define REV_IMP_VOLT_LIM        0x0C    // Reverse_Mode_Input_Voltage_Limit
#define PRECHARGE_CURR_LIM      0x10    // Precharge_Current_Limit
#define TERM_CURR_LIM           0x12    // Termination_Current_Limit

#define VAC_MAX_POW_POINT       0x1F    // VAC_Max_Power_Point_Detected

#define IAC_ADC                 0x2D    // IAC_ADC Register
#define IBAT_ADC                0x2F    // IBAT_ADC Register
#define VAC_ADC                 0x31    // VAC_ADC Register
#define VBAT_ADC                0x33    // VBAT_ADC Register
#define TS_ADC                  0x37    // TS_ADC register
#define VFB_ADC                 0x39    // VFB_ADC Register


// THESE REGISTERS READ 8 BITS
#define PRECHARGE_TERM_CONT     0x14    // Precharge_and_Termination_Control 
#define TIME_CONT               0x15    // Timer_Control Register
#define THR_STG_CHARGE_CONT     0x16    // Three-Stage_Charge_Control 
#define CHARGER_CONT            0x17    // Charger_Control
#define PIN_CONT_REG            0x18    // Pin_Control Register
#define POW_PATH_REV_CONT       0x19    // Power_Path_and_Reverse_Mode_Control
#define MPPT_CONT               0x1A    // MPPT_Control Register
#define CHARGE_THRESH_CONT      0x1B    // Charging_Threshold_Control
#define CHARGE_REGION_CONT      0x1C    // Charging_Region_Behavior_Control
#define REV_THRESH_CONT         0x1D    // Reverse_Mode_Threshold_Control
#define REV_UNDERVOLT_CONT      0x1E    // Reverse_Undervoltage_Control
#define CHARGER_STATUS_1        0x21    // Charger_Status_1 
#define CHARGER_STATUS_2        0x22    // Charger_Status_2
#define CHARGER_STATUS_3        0x23    // Charger_Status_3
#define FAULT_STATUS            0x24    // Fault_Status
#define CHARGER_FLAG_1          0x25    // Charger_Flag_1
#define CHARGER_FLAG_2          0x26    // Charger_Flag_2
#define FAULT_FLAG              0x27    // Fault_Flag Register
#define CHARGER_MASK_1          0x28    // Charger_Mask_1
#define CHARGER_MASK_2          0x29    // Charger_Mask_2
#define FAULT_MASK              0x2A    // Fault_Mask
#define ADC_CONT                0x2B    // ADC_Control Register
#define ADC_CHANNEL_CONT        0x2C    // ADC_Channel_Control Register

#define GATE_DRV_STRG_CONT      0x3B    // Gate_Driver_Strength_Control
#define GATE_DRV_DEATTIME_CONT  0x3C    // Gate_Driver_Dead_Time_Control
#define PART_INFO               0x3D    // Part_Information Register
#define REV_BATT_DISCHARGE_CURR 0x62    // Reverse_Mode_Battery_Discharge_Current