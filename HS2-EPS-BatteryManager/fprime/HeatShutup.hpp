#ifndef BQ25756_HEAT_SHUTUP_HPP
#define BQ25756_HEAT_SHUTUP_HPP

#include "i2c.hpp"
#include "BQ25756_reg.h"

namespace BQ25756 {

/**
 * @brief Thermal shutdown configuration and JEITA zone control.
 */
class HeatShutup {
  public:
    enum TS_LVL     { TS_NORMAL, TS_WARM, TS_COOL, TS_COLD, TS_HOT, TS_INVALID };
    enum TS_T5_prcnt { T5_41p2, T5_37p7, T5_34p375, T5_31p25 };
    enum TS_T3_prcnt { T3_48p4, T3_44p8, T3_41p2, T3_37p7 };
    enum TS_T2_prcnt { T2_71p1, T2_68p4, T2_65p5, T2_62p4 };
    enum TS_T1_prcnt { T1_77p15, T1_75p32, T1_73p25, T1_71p1 };

    TS_LVL readTS_STAT             (const I2cContext& ctx);
    float  readTSVoltagePercent    (const I2cContext& ctx);
    void   JEITA_enable            (const I2cContext& ctx);
    void   JEITA_disable           (const I2cContext& ctx);
    void   TS_enable               (const I2cContext& ctx);
    void   TS_disable              (const I2cContext& ctx);
    void   configure_TS_T5_Charging_Threshold (const I2cContext& ctx, TS_T5_prcnt userInput);
    void   configure_TS_T3_Charging_Threshold (const I2cContext& ctx, TS_T3_prcnt userInput);
    void   configure_TS_T2_Charging_Threshold (const I2cContext& ctx, TS_T2_prcnt userInput);
    void   configure_TS_T1_Charging_Threshold (const I2cContext& ctx, TS_T1_prcnt userInput);
    bool   isJEITAdisabled         (const I2cContext& ctx);
    bool   isTSdisabled            (const I2cContext& ctx);
    void   reset_TS_lvl            (const I2cContext& ctx);
};

}  // namespace BQ25756

#endif  // BQ25756_HEAT_SHUTUP_HPP
