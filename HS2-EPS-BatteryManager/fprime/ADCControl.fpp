module BQ25756 {

    @ ADC enable/disable control for all BQ25756 ADC channels.
    @ Wraps ADC_CONT and ADC_CHANNEL_CONT register operations.
    passive component ADCControl {

        # -------------------------------------------------------------------
        # I2C bus ports — wired to Drv.LinuxI2cDriver in topology
        # -------------------------------------------------------------------

        @ Write-then-read port — used for all register reads
        sync output port busWriteRead: Drv.I2cWriteRead

        @ Write-only port — used for all register writes
        sync output port busWrite: Drv.I2cWrite

        # -------------------------------------------------------------------
        # Command input ports — called by application or ground system
        # -------------------------------------------------------------------

        @ Set ADC conversion rate to continuous
        sync input port setADCContinuous:    BQ25756.CmdNoArgPort

        @ Enable ADC (ADC_CONT bit 7)
        sync input port enableADC:           BQ25756.CmdNoArgPort

        @ Enable all ADC channels at once
        sync input port enableAllADCControl: BQ25756.CmdNoArgPort

        @ Disable ADC
        sync input port disableADC:          BQ25756.CmdNoArgPort

        @ Enable individual ADC channels
        sync input port enableIAC_ADC:       BQ25756.CmdNoArgPort
        sync input port enableIBAT_ADC:      BQ25756.CmdNoArgPort
        sync input port enableVAC_ADC:       BQ25756.CmdNoArgPort
        sync input port enableVBAT_ADC:      BQ25756.CmdNoArgPort
        sync input port enableTS_ADC:        BQ25756.CmdNoArgPort
        sync input port enableVFB_ADC:       BQ25756.CmdNoArgPort

        @ Disable individual ADC channels
        sync input port disableIAC_ADC:      BQ25756.CmdNoArgPort
        sync input port disableIBAT_ADC:     BQ25756.CmdNoArgPort
        sync input port disableVAC_ADC:      BQ25756.CmdNoArgPort
        sync input port disableVBAT_ADC:     BQ25756.CmdNoArgPort
        sync input port disableTS_ADC:       BQ25756.CmdNoArgPort
        sync input port disableVFB_ADC:      BQ25756.CmdNoArgPort

        @ Query ADC status — returns bool
        sync input port isADCEnabled:        BQ25756.QueryBoolPort
        sync input port isADCRateOneshot:    BQ25756.QueryBoolPort
        sync input port isIBAT_ADCDisabled:  BQ25756.QueryBoolPort
        sync input port isIAC_ADCDisabled:   BQ25756.QueryBoolPort
        sync input port isVAC_ADCDisabled:   BQ25756.QueryBoolPort
        sync input port isVBAT_ADCDisabled:  BQ25756.QueryBoolPort
        sync input port isTS_ADCDisabled:    BQ25756.QueryBoolPort
        sync input port isVFB_ADCDisabled:   BQ25756.QueryBoolPort

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

        @ ADC enabled flag
        telemetry AdcEnabled:      bool id 0

        @ ADC conversion rate (true = one-shot, false = continuous)
        telemetry AdcRateOneshot:  bool id 1

        @ IAC ADC channel disabled flag
        telemetry IacAdcDisabled:  bool id 2

        @ IBAT ADC channel disabled flag
        telemetry IbatAdcDisabled: bool id 3

        @ VAC ADC channel disabled flag
        telemetry VacAdcDisabled:  bool id 4

        @ VBAT ADC channel disabled flag
        telemetry VbatAdcDisabled: bool id 5

        @ TS ADC channel disabled flag
        telemetry TsAdcDisabled:   bool id 6

        @ VFB ADC channel disabled flag
        telemetry VfbAdcDisabled:  bool id 7

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        @ ADC enabled successfully
        event AdcEnabled severity activity low \
            format "ADC enabled"

        @ ADC disabled
        event AdcDisabled severity activity low \
            format "ADC disabled"

        @ All ADC channels enabled
        event AllChannelsEnabled severity activity low \
            format "All ADC channels enabled"

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
