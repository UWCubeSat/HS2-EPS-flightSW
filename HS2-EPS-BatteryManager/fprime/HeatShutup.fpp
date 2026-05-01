module BQ25756 {

    @ Thermal shutdown and JEITA zone control for BQ25756.
    passive component HeatShutup {

        # -------------------------------------------------------------------
        # I2C bus ports
        # -------------------------------------------------------------------

        @ Write-then-read port for register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only port for register writes
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Scheduler — periodic thermal telemetry
        # -------------------------------------------------------------------

        @ Periodic update — reads TS status and voltage, publishes telemetry
        sync input port schedIn: Svc.Sched

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

        @ Enable JEITA temperature zone charging profile (CHARGE_REGION_CONT bit 1)
        guarded command HS_JEITA_ENABLE

        @ Disable JEITA temperature zone charging profile (CHARGE_REGION_CONT bit 1)
        guarded command HS_JEITA_DISABLE

        @ Enable TS pin thermistor function (CHARGE_REGION_CONT bit 0)
        guarded command HS_TS_ENABLE

        @ Disable TS pin thermistor function (CHARGE_REGION_CONT bit 0)
        guarded command HS_TS_DISABLE

        # -------------------------------------------------------------------
        # Telemetry — updated every schedIn tick
        # -------------------------------------------------------------------

        @ Current thermal zone from CHARGER_STATUS_2 bits [6:4]
        telemetry ThermalZone: BQ25756.TS_LVL id 0

        @ TS pin voltage as percentage of REGN
        telemetry TSVoltagePercent: F32 id 1 format "{} %%"

        @ JEITA profile enabled flag
        telemetry JEITAEnabled: bool id 2

        @ TS pin function enabled flag
        telemetry TSEnabled: bool id 3

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        event JEITAEnabled \
            severity activity low \
            format "JEITA profile enabled"

        event JEITADisabled \
            severity activity low \
            format "JEITA profile disabled"

        event TSEnabled \
            severity activity low \
            format "TS pin function enabled"

        event TSDisabled \
            severity activity low \
            format "TS pin function disabled"

        event ThermalZoneChanged(zone: BQ25756.TS_LVL) \
            severity activity low \
            format "Thermal zone changed to {}"

        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"

        event I2cWriteFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C write failed on reg 0x{x}: status {}"
    }

}
