cmake_minimum_required(VERSION 3.20)
# Define the project for checking the BQ25756 C++ library
# No executable is created; this project is only for compile-time validation

project(HS2-EPS-BatteryManager VERSION 1.0 LANGUAGES CXX)

add_library(bq25756_obj OBJECT
  src/BQ25756.cpp
  src/i2c.cpp
  src/BatteryMonitor.cpp
  src/HeatShutup.cpp
  src/SafetyConfiguration.cpp
  src/FaultStatus.cpp
)

target_include_directories(bq25756_obj PUBLIC
  include
  stubs
)

target_compile_features(bq25756_obj PUBLIC cxx_std_17)


target_compile_options(bq25756_obj PRIVATE
  -Wall -Wextra -Wpedantic
)
