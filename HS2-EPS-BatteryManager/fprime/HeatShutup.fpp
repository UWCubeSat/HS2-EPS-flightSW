module BQ25756 {

    @ Thermal shutdown configuration and JEITA temperature zone control.
    passive component HeatShutup {

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

        @ Read current thermal zone status
        sync input port getTS_Status:    BQ25756.GetTsLvlPort

        @ Read TS pin voltage as percentage of REGN
        sync input port getTSVoltage:    BQ25756.GetF32Port

        @ Enable/disable JEITA profile
        sync input port enableJEITA:     BQ25756.CmdNoArgPort
        sync input port disableJEITA:    BQ25756.CmdNoArgPort

        @ Enable/disable TS pin function
        sync input port enableTS:        BQ25756.CmdNoArgPort
        sync input port disableTS:       BQ25756.CmdNoArgPort

        @ Configure temperature zone thresholds
        sync input port setT5Threshold:  BQ25756.SetT5Port
        sync input port setT3Threshold:  BQ25756.SetT3Port
        sync input port setT2Threshold:  BQ25756.SetT2Port
        sync input port setT1Threshold:  BQ25756.SetT1Port

        @ Reset all thresholds to factory defaults
        sync input port resetTS:         BQ25756.CmdNoArgPort

        @ Query JEITA and TS disabled status
        sync input port isJEITAdisabled: BQ25756.QueryBoolPort
        sync input port isTSdisabled:    BQ25756.QueryBoolPort

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

        @ Current thermal zone
        telemetry ThermalZone:     BQ25756.TS_LVL id 0

        @ TS pin voltage as percentage of REGN
        telemetry TSVoltage:       F32 id 1 format "{} %%"

        @ JEITA profile enabled flag
        telemetry JEITAEnabled:    bool id 2

        @ TS function enabled flag
        telemetry TSEnabled:       bool id 3

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        @ Thermal zone changed
        event ThermalZoneChanged(zone: BQ25756.TS_LVL) \
            severity activity low \
            format "Thermal zone changed to {}"

        @ JEITA enabled
        event JEITAEnabled severity activity low \
            format "JEITA profile enabled"

        @ JEITA disabled
        event JEITADisabled severity activity low \
            format "JEITA profile disabled"

        @ TS thresholds reset to defaults
        event TSThresholdsReset severity activity low \
            format "TS thresholds reset to factory defaults"

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
