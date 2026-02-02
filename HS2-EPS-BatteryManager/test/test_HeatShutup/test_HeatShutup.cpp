#include "BQ25756.h"
#include "HeatShutup.h"
BQ25756 bq;

struct HsControlState {
    BQ25756::HeatShutup::TS_LVL hs_ts_lvl;
    BQ25756::HeatShutup::TS_T5_prcnt Hs_ts_t5;
    BQ25756::HeatShutup::TS_T3_prcnt Hs_ts_t3;
    BQ25756::HeatShutup::TS_T2_prcnt Hs_ts_t2;
    BQ25756::HeatShutup::TS_T1_prcnt Hs_ts_t1;
    BQ25756::HeatShutup::TS_LVL test_Hs_status;
    
};

static HsControlState HsReadControlState (){
    

}