module BQ25756 {

    @ ADC enable/disable and rate control for BQ25756.
    @
    @ BatteryMonitor uses continuous mode via schedIn.
    @ All other components trigger one-shot reads via commands.
    passive component ADCControl {

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

        @ Enable ADC (sets ADC_CONT bit 7)
        guarded command ADC_ENABLE

        @ Disable ADC (clears ADC_CONT bit 7)
        guarded command ADC_DISABLE

        @ Set ADC conversion rate.
        @ Use CONTINUOUS for BatteryMonitor schedIn.
        @ Use ONESHOT for commanded single reads.
        guarded command ADC_SET_RATE(
            rate: BQ25756.AdcRate  @< CONTINUOUS or ONESHOT
        )

        @ Enable all ADC channels at once (writes 0x00 to ADC_CHANNEL_CONT)
        guarded command ADC_ENABLE_ALL_CHANNELS

        @ Enable a specific ADC channel
        guarded command ADC_ENABLE_CHANNEL(
            channel: BQ25756.AdcChannel
        )

        @ Disable a specific ADC channel
        guarded command ADC_DISABLE_CHANNEL(
            channel: BQ25756.AdcChannel
        )

        # -------------------------------------------------------------------
        # Events
        # -------------------------------------------------------------------

        event AdcEnabled \
            severity activity low \
            format "ADC enabled"

        event AdcDisabled \
            severity activity low \
            format "ADC disabled"

        event AdcRateSet(rate: BQ25756.AdcRate) \
            severity activity low \
            format "ADC rate set to {}"

        event AllChannelsEnabled \
            severity activity low \
            format "All ADC channels enabled"

        event ChannelEnabled(channel: BQ25756.AdcChannel) \
            severity activity low \
            format "ADC channel {} enabled"

        event ChannelDisabled(channel: BQ25756.AdcChannel) \
            severity activity low \
            format "ADC channel {} disabled"

        event I2cReadFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C read failed on reg 0x{x}: status {}"

        event I2cWriteFailed(reg: U8, status: I32) \
            severity warning high \
            format "I2C write failed on reg 0x{x}: status {}"
    }

}
