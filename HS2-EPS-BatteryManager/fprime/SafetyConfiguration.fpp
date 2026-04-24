module BQ25756 {

    @ Charge safety limits, watchdog, MPPT, and EN_CHG control.
    passive component SafetyConfig {

        # -------------------------------------------------------------------
        # I2C bus ports
        # -------------------------------------------------------------------

        @ Write-then-read port — used for all register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only port — used for register writes
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Input ports
        # -------------------------------------------------------------------

        @ Watchdog timer control
        sync input port disableWatchdog:        BQ25756.CmdNoArgPort
        sync input port isWatchdogDisabled:     BQ25756.QueryBoolPort

        @ MPPT control
        sync input port enableMPPT:             BQ25756.CmdNoArgPort
        sync input port disableMPPT:            BQ25756.CmdNoArgPort
        sync input port isMPPTEnabled:          BQ25756.QueryBoolPort

        @ TS pin function control
        sync input port enableTSPin:            BQ25756.CmdNoArgPort
        sync input port isTSPinEnabled:         BQ25756.QueryBoolPort

        @ INT flag status queries
        sync input port isMPPTFlagNormal:       BQ25756.QueryBoolPort
        sync input port isTSFlagNormal:         BQ25756.QueryBoolPort
        sync input port isPGFlagNormal:         BQ25756.QueryBoolPort

        @ Charge enable control
        sync input port isChargingEnabled:      BQ25756.QueryBoolPort
        sync input port interruptCharging:      BQ25756.CmdNoArgPort
        sync input port enableCharging:         BQ25756.CmdNoArgPort

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

        @ Watchdog timer disabled flag
        telemetry WatchdogDisabled:  bool id 0

        @ MPPT enabled flag
        telemetry MPPTEnabled:       bool id 1

        @ TS pin function enabled flag
        telemetry TSPinEnabled:      bool id 2

        @ Charging enabled flag
        telemetry ChargingEnabled:   bool id 3

        @ Power Good INT flag normal
        telemetry PGFlagNormal:      bool id 4

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        @ Charging interrupted
        event ChargingInterrupted severity warning low \
            format "Charging interrupted by command"

        @ Charging enabled
        event ChargingEnabled severity activity low \
            format "Charging enabled"

        @ MPPT enabled
        event MPPTEnabled severity activity low \
            format "MPPT enabled"

        @ MPPT disabled
        event MPPTDisabled severity activity low \
            format "MPPT disabled"

        @ Power Good flag asserted (input power lost or fault)
        event PGFaultDetected severity warning high \
            format "Power Good flag asserted — check input power"

        @ I2C read failed
        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"

        @ I2C write failed
        event I2cWriteFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C write failed on reg 0x{x}: status {}"
    }

}
