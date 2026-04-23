####
# BQ25756/CMakeLists.txt
####

set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/i2c.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/BatteryMonitor.cpp"
)

set(MOD_DEPS
    Drv_I2cDriverPorts   # Drv::I2cStatus, Drv.I2cWriteRead, Drv.I2cWrite
    Fw_Buffer            # Fw::Buffer
    Fw_Log
    Fw_Time
    Fw_Tlm
)

register_fprime_module()
