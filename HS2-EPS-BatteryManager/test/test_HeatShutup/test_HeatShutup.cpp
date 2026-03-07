#include "BQ25756.h"
#include "HeatShutup.h"
BQ25756 bq;

struct HsControlState {
    BQ25756::HeatShutup::TS_LVL ts_status;
    float Hs_ts_adc;
    bool Hs_jeita_enabled;
    bool Hs_ts_enabled;
    
};

static HsControlState HsReadControlState() {
    HsControlState s{};
    s.Hs_ts_adc = bq.hs.readTSVoltagePercent();
    s.ts_status = bq.hs.readTS_STAT();
    s.Hs_jeita_enabled = !bq.hs.isJEITAdisabled();
    s.Hs_ts_enabled = !bq.hs.isTSdisabled();
    return s;
}

static const char* tsLvlStr(BQ25756::HeatShutup::TS_LVL level)
{
    using L = BQ25756::HeatShutup::TS_LVL;
    switch (level) {
        case L::TS_NORMAL:  
            return "NORMAL";
        case L::TS_WARM:    
            return "WARM";
        case L::TS_COOL:    
            return "COOL";
        case L::TS_COLD:    
            return "COLD";
        case L::TS_HOT:     
            return "HOT";
        default:            
            return "INVALID";
    }
}

static void printHsControlState(const HsControlState& s){
    printf(
        "HS | TS:%s ADC:%0.2f%% | JEITA:%s TS_EN:%s\n",
        tsLvlStr(s.ts_status),
        s.Hs_ts_adc,
        s.Hs_jeita_enabled ? "EN" : "DIS",
        s.Hs_ts_enabled ? "EN" : "DIS"
    );
}

void print_state()
{
    printHsControlState(HsReadControlState());
}

void test_JEITA_enable (){
    printf("\n[TEST] JEITA Enable\n");

    printf("Before: ");
    printHsControlState(HsReadControlState());

    bq.hs.JEITA_enable();

    printf("After : ");
    printHsControlState(HsReadControlState());
}

//works
void test_TS_enable(){
    printf("\n[TEST] TS Enable\n");

    printf("Before: ");
    bq.adc.enableTS_ADC();
    bq.adc.enableADC();
    printHsControlState(HsReadControlState());

    bq.hs.TS_enable();

    printf("After : ");
    printHsControlState(HsReadControlState());
}

//works
void test_JEITA_disable (){
    printf("\n[TEST] JEITA Disable\n");

    printf("Before: ");
    printHsControlState(HsReadControlState());

    bq.hs.JEITA_disable();

    printf("After : ");
    printHsControlState(HsReadControlState());
}

//works
void test_is_JEITAdisabled(){
    printf("\n[TEST] is JEITA disabled\n");
    bool dis = bq.hs.isJEITAdisabled();
    printf("isJEITAdisabled() = %s\n", dis ? "TRUE" : "FALSE");
}

//works
void test_TS_disable (){
    printf("\n[TEST] TS Disable\n");

    printf("Before: ");
    printHsControlState(HsReadControlState());

    bq.hs.TS_disable();

    printf("After : ");
    printHsControlState(HsReadControlState());
}

//works
void test_is_TSdisabled (){
    printf("\n[TEST] is TS disabled\n");
    bool dis = bq.hs.isTSdisabled();
    printf("isTSdisabled() = %s\n", dis ? "TRUE" : "FALSE");
}

//doesn't work
//returns: ?
void test_readTSVoltagePercent()
{
    printf("\n[TEST] TS Voltage Percent\n");
    float v = bq.hs.readTSVoltagePercent();
    printf("TS_VOLT_PERCENT = %0.2f%%\n", v);
}

//works
void test_readTS_STAT()
{
    printf("\n[TEST] TS STATUS\n");
    auto st = bq.hs.readTS_STAT();
    printf("TS_STATUS = %s\n", tsLvlStr(st));
}

static const char* t5Str(BQ25756::HeatShutup::TS_T5_prcnt lvl)
{
  using T = BQ25756::HeatShutup::TS_T5_prcnt;
  switch (lvl) {
    case T::T5_41p2:     
        return "T5_41p2 (41.2%)";
    case T::T5_37p7:     
        return "T5_37p7 (37.7%)";
    case T::T5_34p375:   
        return "T5_34p375 (34.375%)";
    case T::T5_31p25:       
        return "T5_31p25 (31.25%)";
    default:             
        return "T5_INVALID";
  }
}

static const char* t3Str(BQ25756::HeatShutup::TS_T3_prcnt lvl)
{
  using T = BQ25756::HeatShutup::TS_T3_prcnt;
  switch (lvl) {
    case T::T3_48p4: 
        return "T3_48p4 (48.4%)";
    case T::T3_44p8: 
        return "T3_44p8 (44.8%)";
    case T::T3_41p2: 
        return "T3_41p2 (41.2%)";
    case T::T3_37p7: 
        return "T3_37p7 (37.7%)";
    default:         
        return "T3_INVALID";
  }
}

static const char* t2Str(BQ25756::HeatShutup::TS_T2_prcnt lvl)
{
  using T = BQ25756::HeatShutup::TS_T2_prcnt;
  switch (lvl) {
    case T::T2_71p1: 
        return "T2_71p1 (71.1%)";
    case T::T2_68p4: 
        return "T2_68p4 (68.4%)";
    case T::T2_65p5: 
        return "T2_65p5 (65.5%)";
    case T::T2_62p4: 
        return "T2_62p4 (62.4%)";
    default:         
        return "T2_INVALID";
  }
}

static const char* t1Str(BQ25756::HeatShutup::TS_T1_prcnt lvl)
{
  using T = BQ25756::HeatShutup::TS_T1_prcnt;
  switch (lvl) {
    case T::T1_77p15: 
        return "T1_77p15 (77.15%)";
    case T::T1_75p32: 
        return "T1_75p32 (75.32%)";
    case T::T1_73p25: 
        return "T1_73p25 (73.25%)";
    case T::T1_71p1:  
        return "T1_71p1 (71.1%)";
    default:          
        return "T1_INVALID";
  }
}

template <typename EnumT>
static void print_TS_levels(
    const char* label,
    EnumT selected,
    const char* (*toStr)(EnumT),
    EnumT o0, EnumT o1, EnumT o2, EnumT o3)
{
    printf("%s Selected: %s\n", label, toStr(selected));
    printf("%s Options : %s%s, %s%s, %s%s, %s%s\n",
            label,
            (selected == o0 ? "*" : " "), toStr(o0),
            (selected == o1 ? "*" : " "), toStr(o1),
            (selected == o2 ? "*" : " "), toStr(o2),
            (selected == o3 ? "*" : " "), toStr(o3));
}

static void test_TS_levels(const char* tag = nullptr)
{
    uint8_t thresh = read8bitRegister(CHARGE_THRESH_CONT);

    auto t5 = static_cast<BQ25756::HeatShutup::TS_T5_prcnt>((thresh >> 6) & 0x03);
    auto t3 = static_cast<BQ25756::HeatShutup::TS_T3_prcnt>((thresh >> 4) & 0x03);
    auto t2 = static_cast<BQ25756::HeatShutup::TS_T2_prcnt>((thresh >> 2) & 0x03);
    auto t1 = static_cast<BQ25756::HeatShutup::TS_T1_prcnt>((thresh >> 0) & 0x03);

    if (tag) printf("\n[%s]\n", tag);
    printf("CHARGE_THRESH_CONT = 0x%02X\n", thresh);

    print_TS_levels("T5", t5, t5Str,
        BQ25756::HeatShutup::TS_T5_prcnt::T5_41p2,
        BQ25756::HeatShutup::TS_T5_prcnt::T5_37p7,
        BQ25756::HeatShutup::TS_T5_prcnt::T5_34p375,
        BQ25756::HeatShutup::TS_T5_prcnt::T5_31p25);

    print_TS_levels("T3", t3, t3Str,
        BQ25756::HeatShutup::TS_T3_prcnt::T3_48p4,
        BQ25756::HeatShutup::TS_T3_prcnt::T3_44p8,
        BQ25756::HeatShutup::TS_T3_prcnt::T3_41p2,
        BQ25756::HeatShutup::TS_T3_prcnt::T3_37p7);

    print_TS_levels("T2", t2, t2Str,
        BQ25756::HeatShutup::TS_T2_prcnt::T2_71p1,
        BQ25756::HeatShutup::TS_T2_prcnt::T2_68p4,
        BQ25756::HeatShutup::TS_T2_prcnt::T2_65p5,
        BQ25756::HeatShutup::TS_T2_prcnt::T2_62p4);

    print_TS_levels("T1", t1, t1Str,
        BQ25756::HeatShutup::TS_T1_prcnt::T1_77p15,
        BQ25756::HeatShutup::TS_T1_prcnt::T1_75p32,
        BQ25756::HeatShutup::TS_T1_prcnt::T1_73p25,
        BQ25756::HeatShutup::TS_T1_prcnt::T1_71p1);

}

//works
void test_configure_TS_T5_Charging_Threshold (BQ25756::HeatShutup::TS_T5_prcnt lvl){
    printf("\n[TEST] Set T5\n");
    test_TS_levels("Before");

    bq.hs.configure_TS_T5_Charging_Threshold(lvl);

    test_TS_levels("After");
}

//works
void test_configure_TS_T3_Charging_Threshold (BQ25756::HeatShutup::TS_T3_prcnt lvl){
    printf("\n[TEST] Set T3\n");
    test_TS_levels("Before");

    bq.hs.configure_TS_T3_Charging_Threshold(lvl);

    test_TS_levels("After");
}

void test_configure_TS_T2_Charging_Threshold (BQ25756::HeatShutup::TS_T2_prcnt lvl){
    printf("\n[TEST] Set T2\n");
    test_TS_levels("Before");

    bq.hs.configure_TS_T2_Charging_Threshold(lvl);

    test_TS_levels("After");
}

//works
void test_configure_TS_T1_Charging_Threshold (BQ25756::HeatShutup::TS_T1_prcnt lvl){
    printf("\n[TEST] Set T1\n");
    test_TS_levels("Before");

    bq.hs.configure_TS_T1_Charging_Threshold(lvl);

    test_TS_levels("After");
}

void test_reset_TS_lvl(){
    printf("\n[TEST] Reset Jeita Levels\n");
    test_TS_levels("Before");

    bq.hs.reset_TS_lvl();

    test_TS_levels("After");
}



