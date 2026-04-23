module BQ25756 {

    @ All battery measurements in one struct
    struct BatteryMeasurements {
        vac:       I32,
        vbat:      I32,
        vfb:       I32,
        vrechg:    I32,
        vbat_lowv: I32,
        ichg:      I32,
        iac:       I32,
        ibat:      I32
    }

    @ Charging status — mirrors CHARGER_STATUS_1 register bits [2:0]
    enum ChargingStatus : U8 {
        NOT_CHARGING            = 0,
        TRICKLE_CHARGE          = 1,
        PRE_CHARGE              = 2,
        FAST_CHARGE             = 3,
        TAPER_CHARGE            = 4,
        RESERVED                = 5,
        TOP_OFF_TIMER_ACTIVE    = 6,
        CHARGE_TERMINATION_DONE = 7
    }

    passive component BatteryMonitor {

        # -------------------------------------------------------------------
        # I2C output ports — wired to Drv.LinuxI2cDriver in topology
        # -------------------------------------------------------------------

        @ Write-then-read (repeated START) — used for all register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only — used for register writes
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Input ports — called by higher-level application components
        # -------------------------------------------------------------------

        sync input port getMeasurements:  BQ25756.GetMeasurementsPort
        sync input port getChargingStatus: BQ25756.GetChargingStatusPort
        sync input port getVac:  BQ25756.GetIntValuePort
        sync input port getVbat: BQ25756.GetIntValuePort
        sync input port getVfb:  BQ25756.GetIntValuePort
        sync input port getIac:  BQ25756.GetIntValuePort
        sync input port getIbat: BQ25756.GetIntValuePort

        # -------------------------------------------------------------------
        # Framework ports
        # -------------------------------------------------------------------

        telemetry port tlmOut
        event port logOut
        text event port logTextOut
        time get port timeGetOut

        # -------------------------------------------------------------------
        # Telemetry
        # -------------------------------------------------------------------

        telemetry Vac:      I32 id 0 format "{} mV"
        telemetry Vbat:     I32 id 1 format "{} mV"
        telemetry Vfb:      I32 id 2 format "{} mV"
        telemetry Iac:      I32 id 3 format "{} mA"
        telemetry Ibat:     I32 id 4 format "{} mA"
        telemetry Ichg:     I32 id 5 format "{} mA"
        telemetry Vrechg:   I32 id 6 format "{} mV"
        telemetry VbatLowv: I32 id 7 format "{} mV"

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"

        event I2cWriteFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C write failed on reg 0x{x}: status {}"

        event InvalidRegisterOption(reg: U8, option: I32) \
            severity warning low \
            format "Invalid option {} read from reg 0x{x}"
    }

}
