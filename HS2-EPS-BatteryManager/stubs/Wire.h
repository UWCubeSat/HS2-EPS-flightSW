// This is the dummy file for Wire.h for CMake
#pragma once
#include <cstdint>
#include <cstddef>

// Minimal stub for Arduino TwoWire (I2C) to allow compilation on a host PC
class TwoWire {
public:
  void begin() {}
  void setClock(uint32_t) {}

  void beginTransmission(uint8_t) {}
  size_t write(uint8_t) { return 1; }

  // Return 0 to mimic "success"
  uint8_t endTransmission(bool = true) { return 0; }

  uint8_t requestFrom(uint8_t, uint8_t) { return 0; }

  int available() { return 0; }
  int read() { return 0; }
};

extern TwoWire Wire;
