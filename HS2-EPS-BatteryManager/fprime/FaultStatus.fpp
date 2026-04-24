module BQ25756 {

    @ Fault flag register reader for BQ25756.
    @ Each port returns true when the fault is unmasked (will generate INT pulse).
    passive component FaultStatus {

        # -------------------------------------------------------------------
        # I2C bus ports
        # -------------------------------------------------------------------

        @ Write-then-read port — used for all register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only port — not used for reads but required by I2c interface
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Input ports — one per fault bit in FAULT_MASK register
        # -------------------------------------------------------------------

        @ Input under-voltage fault (FAULT_MASK bit 7)
        sync input port checkInputUnderVoltage:  BQ25756.QueryBoolPort

        @ Input over-voltage fault (FAULT_MASK bit 6)
        sync input port checkInputOverVoltage:   BQ25756.QueryBoolPort

        @ Battery over-current fault (FAULT_MASK bit 5)
        sync input port checkBattOverCurrent:    BQ25756.QueryBoolPort

        @ Battery over-voltage fault (FAULT_MASK bit 4)
        sync input port checkBattOverVoltage:    BQ25756.QueryBoolPort

        @ Thermal shutdown fault (FAULT_MASK bit 3)
        sync input port checkThermalShutdown:    BQ25756.QueryBoolPort

        @ Charge safety timer fault (FAULT_MASK bit 2)
        sync input port checkSafetyTimer:        BQ25756.QueryBoolPort

        @ DRV_SUP pin fault (FAULT_MASK bit 1)
        sync input port checkDRV_SUPPin:         BQ25756.QueryBoolPort

        # -------------------------------------------------------------------
        # Framework ports
        # -------------------------------------------------------------------

        telemetry port tlmOut
        event port logOut
        text event port logTextOut
        time get port timeGetOut

        # -------------------------------------------------------------------
        # Telemetry — live snapshot of each fault mask bit
        # -------------------------------------------------------------------

        @ Input under-voltage fault active
        telemetry FaultInputUnderVoltage:  bool id 0

        @ Input over-voltage fault active
        telemetry FaultInputOverVoltage:   bool id 1

        @ Battery over-current fault active
        telemetry FaultBattOverCurrent:    bool id 2

        @ Battery over-voltage fault active
        telemetry FaultBattOverVoltage:    bool id 3

        @ Thermal shutdown fault active
        telemetry FaultThermalShutdown:    bool id 4

        @ Charge safety timer fault active
        telemetry FaultSafetyTimer:        bool id 5

        @ DRV_SUP pin fault active
        telemetry FaultDRV_SUPPin:         bool id 6

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        @ Input under-voltage fault detected
        event InputUnderVoltageFault severity warning high \
            format "BQ25756: Input under-voltage fault detected"

        @ Input over-voltage fault detected
        event InputOverVoltageFault severity warning high \
            format "BQ25756: Input over-voltage fault detected"

        @ Battery over-current fault detected
        event BatteryOverCurrentFault severity warning high \
            format "BQ25756: Battery over-current fault detected"

        @ Battery over-voltage fault detected
        event BatteryOverVoltageFault severity warning high \
            format "BQ25756: Battery over-voltage fault detected"

        @ Thermal shutdown fault detected
        event ThermalShutdownFault severity warning high \
            format "BQ25756: Thermal shutdown fault detected"

        @ Charge safety timer expired
        event ChargeSafetyTimerFault severity warning high \
            format "BQ25756: Charge safety timer expired"

        @ DRV_SUP pin fault detected
        event DRV_SUPPinFault severity warning high \
            format "BQ25756: DRV_SUP pin fault detected"

        @ I2C read failed
        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"
    }

}
