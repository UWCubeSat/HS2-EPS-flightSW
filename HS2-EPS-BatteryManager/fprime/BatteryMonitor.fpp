module BQ25756 {

    @ Battery voltage, current, and charging status monitor.
    @ Reads BQ25756 ADC registers and charging status register.
    passive component BatteryMonitor {

        # -------------------------------------------------------------------
        # I2C bus ports — wired to Drv.LinuxI2cDriver in topology
        # -------------------------------------------------------------------

        @ Write-then-read port — used for all register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only port — used for register writes
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Input ports — called by application components
        # -------------------------------------------------------------------

        @ Get all battery measurements in one call
        sync input port getMeasurements:   BQ25756.GetMeasurementsPort

        @ Get current charging status
        sync input port getChargingStatus: BQ25756.GetChargingStatusPort

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

        @ Input voltage (mV)
        telemetry Vac:      I32 id 0 format "{} mV"

        @ Battery voltage (mV)
        telemetry Vbat:     I32 id 1 format "{} mV"

        @ Feedback voltage (mV)
        telemetry Vfb:      I32 id 2 format "{} mV"

        @ Auto-recharge threshold (mV)
        telemetry Vrechg:   I32 id 3 format "{} mV"

        @ Pre-charge to fast-charge threshold (mV)
        telemetry VbatLowv: I32 id 4 format "{} mV"

        @ Fast charge current limit (mA)
        telemetry Ichg:     I32 id 5 format "{} mA"

        @ Input current (mA)
        telemetry Iac:      I32 id 6 format "{} mA"

        @ Battery current (mA)
        telemetry Ibat:     I32 id 7 format "{} mA"

        @ Current charging status
        telemetry ChargingState: BQ25756.ChargingStatus id 8

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        @ I2C read failed
        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"

        @ An invalid option was decoded from a register
        event InvalidRegisterOption(reg: U8, option: I32) \
            severity warning low \
            format "Invalid option {} read from reg 0x{x}"
    }

}
