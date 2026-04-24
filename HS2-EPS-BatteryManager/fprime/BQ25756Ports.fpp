module BQ25756 {

    # ===========================================================================
    # Shared port types used across all BQ25756 components
    # ===========================================================================

    @ Command port with no arguments (enable/disable/reset operations)
    port CmdNoArgPort()

    @ Query port returning a boolean status
    port QueryBoolPort() -> bool

    @ Query port returning a 32-bit integer (mV or mA)
    port GetIntValuePort() -> I32

    @ Query port returning a float (e.g. TS voltage percentage)
    port GetF32Port() -> F32

    @ Port returning all battery measurements as a struct
    port GetMeasurementsPort() -> BQ25756.BatteryMeasurements

    @ Port returning the current charging status enum
    port GetChargingStatusPort() -> BQ25756.ChargingStatus

    @ Port returning the current thermal zone level
    port GetTsLvlPort() -> BQ25756.TS_LVL

    @ Port for setting T5 zone threshold
    port SetT5Port(threshold: BQ25756.TS_T5_prcnt)

    @ Port for setting T3 zone threshold
    port SetT3Port(threshold: BQ25756.TS_T3_prcnt)

    @ Port for setting T2 zone threshold
    port SetT2Port(threshold: BQ25756.TS_T2_prcnt)

    @ Port for setting T1 zone threshold
    port SetT1Port(threshold: BQ25756.TS_T1_prcnt)

}
