module BQ25756 {

    # ===========================================================================
    # Shared enums
    # ===========================================================================

    @ Charging status from CHARGER_STATUS_1 register bits [2:0]
    enum ChargingStatus : U8 {
        NOT_CHARGING            = 0x00,
        TRICKLE_CHARGE          = 0x01,
        PRE_CHARGE              = 0x02,
        FAST_CHARGE             = 0x03,
        TAPER_CHARGE            = 0x04,
        RESERVED                = 0x05,
        TOP_OFF_TIMER_ACTIVE    = 0x06,
        CHARGE_TERMINATION_DONE = 0x07
    }

    @ Thermal zone status from CHARGER_STATUS_2 bits [6:4]
    enum TS_LVL : U8 {
        TS_NORMAL  = 0,
        TS_WARM    = 1,
        TS_COOL    = 2,
        TS_COLD    = 3,
        TS_HOT     = 4,
        TS_INVALID = 5
    }

    @ T5 zone threshold percentage options
    enum TS_T5_prcnt : U8 {
        T5_41p2   = 0,   @< 41.2%
        T5_37p7   = 1,   @< 37.7%
        T5_34p375 = 2,   @< 34.375%
        T5_31p25  = 3    @< 31.25%
    }

    @ T3 zone threshold percentage options
    enum TS_T3_prcnt : U8 {
        T3_48p4 = 0,     @< 48.4%
        T3_44p8 = 1,     @< 44.8%
        T3_41p2 = 2,     @< 41.2%
        T3_37p7 = 3      @< 37.7%
    }

    @ T2 zone threshold percentage options
    enum TS_T2_prcnt : U8 {
        T2_71p1 = 0,     @< 71.1%
        T2_68p4 = 1,     @< 68.4%
        T2_65p5 = 2,     @< 65.5%
        T2_62p4 = 3      @< 62.4%
    }

    @ T1 zone threshold percentage options
    enum TS_T1_prcnt : U8 {
        T1_77p15 = 0,    @< 77.15%
        T1_75p32 = 1,    @< 75.32%
        T1_73p25 = 2,    @< 73.25%
        T1_71p1  = 3     @< 71.1%
    }

    # ===========================================================================
    # Shared struct
    # ===========================================================================

    @ All battery measurements — voltages in mV, currents in mA
    struct BatteryMeasurements {
        vac:       I32,   @< Input voltage (mV)
        vbat:      I32,   @< Battery voltage (mV)
        vfb:       I32,   @< Feedback voltage (mV)
        vrechg:    I32,   @< Auto-recharge threshold (mV)
        vbat_lowv: I32,   @< Pre-charge to fast-charge threshold (mV)
        ichg:      I32,   @< Fast charge current limit (mA)
        iac:       I32,   @< Input current (mA)
        ibat:      I32    @< Battery current (mA)
    }

}

    @ ADC channel selector — used by ADC_ENABLE_CHANNEL and ADC_DISABLE_CHANNEL commands
    enum AdcChannel : U8 {
        IAC  = 0,   @< Input current ADC (ADC_CHANNEL_CONT bit 7)
        IBAT = 1,   @< Battery current ADC (ADC_CHANNEL_CONT bit 6)
        VAC  = 2,   @< Input voltage ADC (ADC_CHANNEL_CONT bit 5)
        VBAT = 3,   @< Battery voltage ADC (ADC_CHANNEL_CONT bit 4)
        TS   = 4,   @< Temperature sensor ADC (ADC_CHANNEL_CONT bit 2)
        VFB  = 5    @< Feedback voltage ADC (ADC_CHANNEL_CONT bit 1)
    }

    @ ADC conversion rate — used by ADC_SET_RATE command
    enum AdcRate : U8 {
        CONTINUOUS = 0,   @< ADC keeps converting (use with BatteryMonitor schedIn)
        ONESHOT    = 1    @< ADC converts once then stops (use for commanded reads)
    }
