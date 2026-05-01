module BQ25756 {

    @ Charge safety: MPPT control, charging enable/disable, watchdog.
    passive component SafetyConfig {

        # -------------------------------------------------------------------
        # I2C bus ports
        # -------------------------------------------------------------------

        @ Write-then-read port for register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only port for register writes
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Framework ports
        # -------------------------------------------------------------------

        command reg port cmdRegOut
        command recv port cmdIn
        command resp port cmdResponseOut
        telemetry port tlmOut
        event port logOut
        text event port logTextOut
        time get port timeGetOut

        # -------------------------------------------------------------------
        # Commands
        # -------------------------------------------------------------------

        @ Enable MPPT (sets MPPT_CONT bit 0).
        @ When enabled, ADC is controlled by device; writes to REG2A ignored.
        guarded command SC_ENABLE_MPPT

        @ Disable MPPT (clears MPPT_CONT bit 0)
        guarded command SC_DISABLE_MPPT

        @ Stop charging immediately (clears CHARGER_CONT bit 0)
        guarded command SC_INTERRUPT_CHARGING

        @ Enable charging (sets CHARGER_CONT bit 0)
        guarded command SC_ENABLE_CHARGING

        @ Disable watchdog timer (clears TIME_CONT bits [5:4])
        guarded command SC_DISABLE_WATCHDOG

        @ Enable TS pin function control (sets CHARGE_REGION_CONT bit 0)
        guarded command SC_ENABLE_TS_PIN

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        event MPPTEnabled \
            severity activity low \
            format "MPPT enabled — ADC controlled by device"

        event MPPTDisabled \
            severity activity low \
            format "MPPT disabled"

        event ChargingInterrupted \
            severity warning low \
            format "Charging interrupted by command"

        event ChargingEnabled \
            severity activity low \
            format "Charging enabled"

        event WatchdogDisabled \
            severity activity low \
            format "Watchdog timer disabled"

        event TSPinEnabled \
            severity activity low \
            format "TS pin function control enabled"

        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"

        event I2cWriteFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C write failed on reg 0x{x}: status {}"
    }

}
