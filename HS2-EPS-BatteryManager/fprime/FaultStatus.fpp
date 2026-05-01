module BQ25756 {

    @ Fault flag monitor for BQ25756.
    @
    @ FAULT_CHECK command reads FAULT_MASK register and fires a
    @ WARNING_HI event for every active (unmasked) fault bit.
    @ schedIn publishes the raw mask as telemetry every cycle.
    passive component FaultStatus {

        # -------------------------------------------------------------------
        # I2C bus ports
        # -------------------------------------------------------------------

        @ Write-then-read port for register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only port (required by Drv.I2c interface)
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Scheduler — periodic telemetry refresh
        # -------------------------------------------------------------------

        @ Periodic update — reads FAULT_MASK, publishes telemetry, fires events on new faults
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

        @ Read FAULT_MASK register immediately and log all active faults.
        @ Use this for on-demand fault inspection outside the schedule.
        guarded command FAULT_CHECK

        # -------------------------------------------------------------------
        # Telemetry — updated every schedIn tick
        # -------------------------------------------------------------------

        @ Raw FAULT_MASK register value
        @ bit7=InputUnderV, bit6=InputOverV, bit5=BattOverI,
        @ bit4=BattOverV, bit3=ThermalShutdown, bit2=SafetyTimer, bit1=DRV_SUP
        @ 0 = fault generates INT pulse (unmasked/active)
        telemetry FaultMaskRaw: U8 id 0

        # -------------------------------------------------------------------
        # Events — fired when a fault bit is active (bit == 0 = unmasked)
        # -------------------------------------------------------------------

        event InputUnderVoltageFault \
            severity warning high \
            format "BQ25756: Input under-voltage fault active"

        event InputOverVoltageFault \
            severity warning high \
            format "BQ25756: Input over-voltage fault active"

        event BatteryOverCurrentFault \
            severity warning high \
            format "BQ25756: Battery over-current fault active"

        event BatteryOverVoltageFault \
            severity warning high \
            format "BQ25756: Battery over-voltage fault active"

        event ThermalShutdownFault \
            severity warning high \
            format "BQ25756: Thermal shutdown fault active"

        event ChargeSafetyTimerFault \
            severity warning high \
            format "BQ25756: Charge safety timer expired"

        event DRV_SUPPinFault \
            severity warning high \
            format "BQ25756: DRV_SUP pin fault active"

        event NoFaultsDetected \
            severity activity low \
            format "BQ25756: No faults detected"

        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"
    }

}
