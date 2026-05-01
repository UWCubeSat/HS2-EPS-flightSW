# BQ25756 Battery Charger — FPrime Component Suite

FPrime driver suite for the **BQ25756** battery charger IC, communicating over Linux I2C (`/dev/i2c-N`) via `Drv::LinuxI2cDriver`.

---

## Table of Contents

- [Architecture Overview](#architecture-overview)
- [Key Concepts](#key-concepts)
- [Port Summary](#port-summary)
- [Command Summary](#command-summary)
- [Components](#components)
  - [ADCControl](#adccontrol)
  - [BatteryMonitor](#batterymonitor)
  - [FaultStatus](#faultstatus)
  - [HeatShutup](#heatshutup)
  - [SafetyConfig](#safetyconfig)
- [Topology Wiring](#topology-wiring)
- [File Structure](#file-structure)

---

## Architecture Overview

```
Ground Station / Autonomy
        │ commands
        ▼
┌───────────────┐  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐
│  ADCControl   │  │BatteryMonitor │  │  FaultStatus  │  │  HeatShutup   │  │ SafetyConfig  │
│               │  │               │  │               │  │               │  │               │
│ busWriteRead  │  │ busWriteRead  │  │ busWriteRead  │  │ busWriteRead  │  │ busWriteRead  │
│ busWrite      │  │ busWrite      │  │ busWrite      │  │ busWrite      │  │ busWrite      │
└──────┬────────┘  └──────┬────────┘  └──────┬────────┘  └──────┬────────┘  └──────┬────────┘
       │                  │                  │                  │                  │
       └──────────────────┴──────────────────┴──────────────────┴──────────────────┘
                                             │
                                    Drv::LinuxI2cDriver
                                             │
                                      /dev/i2c-1  (BQ25756 addr: 0x6B)
```

All I2C transactions go through `Drv::LinuxI2cDriver` — none of the components contain any Linux `ioctl` calls. The shared `i2c.hpp/cpp` helper layer bridges component port calls to the driver.

---

## Key Concepts

### Commands
Commands are actions sent by a ground operator or autonomy system to trigger a specific operation. Each command generates a response (`OK` or `VALIDATION_ERROR`) confirming whether it succeeded.

```
Ground sends: ADC_ENABLE
    → ADCControl calls enableADC(ctx)
    → writes ADC_CONT bit 7 over I2C
    → responds OK
    → fires AdcEnabled event
```

### schedIn
`schedIn` is a periodic timer port connected to a **rate group**. Components with `schedIn` run their handler automatically every tick (e.g. every 1 second) to refresh telemetry — no ground command needed.

```
RateGroup (1 Hz)
    │
    ├──► BatteryMonitor.schedIn  → reads VAC, VBAT, VFB, IAC, IBAT → publishes telemetry
    ├──► FaultStatus.schedIn     → reads FAULT_MASK → fires WARNING events on active faults
    └──► HeatShutup.schedIn      → reads thermal zone + TS voltage → publishes telemetry
```

### Events
Events are log messages emitted by a component and visible in the ground station.

| Severity | Meaning |
|---|---|
| `ACTIVITY_LO` | Normal informational event (e.g. "ADC enabled") |
| `WARNING_LO` | Non-critical warning (e.g. "Charging interrupted") |
| `WARNING_HI` | Serious fault condition (e.g. "Battery over-voltage fault") |

### Telemetry
Telemetry channels are data values continuously published to the ground station, updated every `schedIn` tick.

### Ports
Ports are the typed connections between FPrime components.

| Direction | Keyword | Meaning |
|---|---|---|
| Input | `sync input port` | Other components call INTO this component |
| Output | `sync output port` | This component calls OUT to another component |

Port number is always `0` since each component has exactly one of each port type.

---

## Port Summary

| Component | Port Name | Direction | Index | Type | Purpose |
|---|---|---|---|---|---|
| ADCControl | `busWriteRead` | output | 0 | `Drv.I2cWriteRead` | Register reads (repeated-START) |
| ADCControl | `busWrite` | output | 0 | `Drv.I2cWrite` | Register writes |
| BatteryMonitor | `busWriteRead` | output | 0 | `Drv.I2cWriteRead` | Register reads |
| BatteryMonitor | `busWrite` | output | 0 | `Drv.I2cWrite` | Register writes |
| BatteryMonitor | `schedIn` | input | 0 | `Svc.Sched` | Periodic telemetry update |
| BatteryMonitor | `getMeasurements` | input | 0 | `BQ25756.GetMeasurementsPort` | On-demand measurement pull |
| BatteryMonitor | `getChargingStatus` | input | 0 | `BQ25756.GetChargingStatusPort` | On-demand charging status pull |
| FaultStatus | `busWriteRead` | output | 0 | `Drv.I2cWriteRead` | Register reads |
| FaultStatus | `busWrite` | output | 0 | `Drv.I2cWrite` | Register writes |
| FaultStatus | `schedIn` | input | 0 | `Svc.Sched` | Periodic fault polling |
| HeatShutup | `busWriteRead` | output | 0 | `Drv.I2cWriteRead` | Register reads |
| HeatShutup | `busWrite` | output | 0 | `Drv.I2cWrite` | Register writes |
| HeatShutup | `schedIn` | input | 0 | `Svc.Sched` | Periodic thermal telemetry |
| SafetyConfig | `busWriteRead` | output | 0 | `Drv.I2cWriteRead` | Register reads |
| SafetyConfig | `busWrite` | output | 0 | `Drv.I2cWrite` | Register writes |

**Total: 15 ports** across all 5 components.

---

## Command Summary

| Component | Command | Arguments | Description |
|---|---|---|---|
| ADCControl | `ADC_ENABLE` | — | Enable ADC |
| ADCControl | `ADC_DISABLE` | — | Disable ADC |
| ADCControl | `ADC_SET_RATE` | `rate: AdcRate` | Set CONTINUOUS or ONESHOT |
| ADCControl | `ADC_ENABLE_ALL_CHANNELS` | — | Enable all 6 ADC channels |
| ADCControl | `ADC_ENABLE_CHANNEL` | `channel: AdcChannel` | Enable one channel |
| ADCControl | `ADC_DISABLE_CHANNEL` | `channel: AdcChannel` | Disable one channel |
| FaultStatus | `FAULT_CHECK` | — | Read and log all active faults immediately |
| HeatShutup | `HS_JEITA_ENABLE` | — | Enable JEITA profile |
| HeatShutup | `HS_JEITA_DISABLE` | — | Disable JEITA profile |
| HeatShutup | `HS_TS_ENABLE` | — | Enable TS pin thermistor |
| HeatShutup | `HS_TS_DISABLE` | — | Disable TS pin thermistor |
| SafetyConfig | `SC_ENABLE_MPPT` | — | Enable MPPT |
| SafetyConfig | `SC_DISABLE_MPPT` | — | Disable MPPT |
| SafetyConfig | `SC_INTERRUPT_CHARGING` | — | Stop charging immediately |
| SafetyConfig | `SC_ENABLE_CHARGING` | — | Enable charging |
| SafetyConfig | `SC_DISABLE_WATCHDOG` | — | Disable watchdog timer |
| SafetyConfig | `SC_ENABLE_TS_PIN` | — | Enable TS pin function control |

**Total: 17 commands** across all 5 components.

> **Note:** HeatShutup threshold configuration (`configure_TS_Tx_Charging_Threshold`, `reset_TS_lvl`) is available as direct C++ calls but not exposed as FPrime commands. Configure thresholds at startup in `Topology.cpp`.

---

## Components

---

### ADCControl

Controls the BQ25756 ADC enable state and channel configuration.
Registers: `ADC_CONT` (0x2B), `ADC_CHANNEL_CONT` (0x2C).

**No schedIn** — ADC configuration is set once at startup and left. Status is event-driven.

#### Commands

| Command | Arguments | Register | Description |
|---|---|---|---|
| `ADC_ENABLE` | — | ADC_CONT bit 7 = 1 | Enable ADC. Call `ADC_SET_RATE` first. |
| `ADC_DISABLE` | — | ADC_CONT bit 7 = 0 | Disable ADC |
| `ADC_SET_RATE` | `rate: AdcRate` | ADC_CONT bit 6 | `CONTINUOUS` (bit=0) for BatteryMonitor schedIn. `ONESHOT` (bit=1) for commanded reads — hardware auto-clears ADC_EN after one conversion. |
| `ADC_ENABLE_ALL_CHANNELS` | — | ADC_CHANNEL_CONT = 0x00 | Enable all 6 channels at once |
| `ADC_ENABLE_CHANNEL` | `channel: AdcChannel` | ADC_CHANNEL_CONT (clear bit) | Enable one channel. `IAC`(bit7) `IBAT`(bit6) `VAC`(bit5) `VBAT`(bit4) `TS`(bit2) `VFB`(bit1) |
| `ADC_DISABLE_CHANNEL` | `channel: AdcChannel` | ADC_CHANNEL_CONT (set bit) | Disable one channel |

#### Events

| Event | Severity | Description |
|---|---|---|
| `AdcEnabled` | ACTIVITY_LO | ADC enabled |
| `AdcDisabled` | ACTIVITY_LO | ADC disabled |
| `AdcRateSet(rate)` | ACTIVITY_LO | Conversion rate changed |
| `AllChannelsEnabled` | ACTIVITY_LO | All channels enabled |
| `ChannelEnabled(channel)` | ACTIVITY_LO | Specific channel enabled |
| `ChannelDisabled(channel)` | ACTIVITY_LO | Specific channel disabled |
| `I2cReadFailed(reg, status)` | WARNING_HI | I2C read failed |
| `I2cWriteFailed(reg, status)` | WARNING_HI | I2C write failed |

#### Typical startup sequence
```
ADC_SET_RATE(CONTINUOUS)       ← set before enabling for BatteryMonitor
ADC_ENABLE_ALL_CHANNELS        ← enable IAC, IBAT, VAC, VBAT, TS, VFB
ADC_ENABLE                     ← start converting
```

---

### BatteryMonitor

Reads all BQ25756 ADC measurement registers and charging status.
Publishes 9 telemetry channels every `schedIn` tick.

**Has schedIn** — automatically reads all ADC registers every rate group cycle.

#### Telemetry

| Channel | ID | Type | Unit | Register | Range / Step |
|---|---|---|---|---|---|
| `Vac` | 0 | I32 | mV | VAC_ADC (0x31) | 0–65534 mV, step 2 mV |
| `Vbat` | 1 | I32 | mV | VBAT_ADC (0x33) | 0–65534 mV, step 2 mV |
| `Vfb` | 2 | I32 | mV | VFB_ADC (0x39) | 0–2047 mV, step 2 mV |
| `Vrechg` | 3 | I32 | mV | CHARGER_CONT (0x17) bits [7:6] | % of VFB_REG |
| `VbatLowv` | 4 | I32 | mV | PRECHARGE_TERM_CONT (0x14) bits [2:1] | % of VFB_REG |
| `Ichg` | 5 | I32 | mA | REG0x02–0x03 bits [10:2] | 400–20000 mA, step 50 mA |
| `Iac` | 6 | I32 | mA | IAC_ADC (0x2D) | ±20000 mA, step 0.8 mA |
| `Ibat` | 7 | I32 | mA | IBAT_ADC (0x2F) | ±20000 mA, step 2 mA |
| `ChargingState` | 8 | ChargingStatus | — | CHARGER_STATUS_1 (0x21) bits [2:0] | See enum below |

#### ChargingStatus enum

| Value | Meaning |
|---|---|
| `NOT_CHARGING` (0) | No active charging |
| `TRICKLE_CHARGE` (1) | Trickle charge phase |
| `PRE_CHARGE` (2) | Pre-charge phase |
| `FAST_CHARGE` (3) | Fast charge phase |
| `TAPER_CHARGE` (4) | Taper charge phase |
| `RESERVED` (5) | Reserved |
| `TOP_OFF_TIMER_ACTIVE` (6) | Top-off timer running |
| `CHARGE_TERMINATION_DONE` (7) | Charging complete |

#### Events

| Event | Severity | Description |
|---|---|---|
| `I2cReadFailed(reg, status)` | WARNING_HI | I2C read failed on specified register |
| `InvalidRegisterOption(reg, option)` | WARNING_LO | Unexpected bit pattern decoded from register |

---

### FaultStatus

Monitors the BQ25756 `FAULT_MASK` register (0x2A).
In BQ25756, a **cleared bit (0)** means the fault is active and generates an INT pulse.

**Has schedIn** — polls `FAULT_MASK` every cycle, fires `WARNING_HI` events automatically.
**Also has `FAULT_CHECK` command** — for on-demand inspection outside the schedule.

#### Commands

| Command | Arguments | Description |
|---|---|---|
| `FAULT_CHECK` | — | Read FAULT_MASK immediately and log all active faults. Same logic as schedIn but triggered on demand. |

#### Telemetry

| Channel | ID | Type | Description |
|---|---|---|---|
| `FaultMaskRaw` | 0 | U8 | Raw FAULT_MASK register. **0 = active fault**, 1 = masked. bit7=InputUnderV, bit6=InputOverV, bit5=BattOverI, bit4=BattOverV, bit3=ThermalShutdown, bit2=SafetyTimer, bit1=DRV_SUP |

#### Events

| Event | Severity | Fault bit | Description |
|---|---|---|---|
| `InputUnderVoltageFault` | WARNING_HI | bit 7 | Input voltage below minimum |
| `InputOverVoltageFault` | WARNING_HI | bit 6 | Input voltage above maximum |
| `BatteryOverCurrentFault` | WARNING_HI | bit 5 | Battery current exceeded limit |
| `BatteryOverVoltageFault` | WARNING_HI | bit 4 | Battery voltage exceeded limit |
| `ThermalShutdownFault` | WARNING_HI | bit 3 | IC temperature exceeded shutdown threshold |
| `ChargeSafetyTimerFault` | WARNING_HI | bit 2 | Safety timer expired before charge completion |
| `DRV_SUPPinFault` | WARNING_HI | bit 1 | DRV_SUP supply pin fault |
| `NoFaultsDetected` | ACTIVITY_LO | — | All fault bits masked, no active faults |
| `I2cReadFailed(reg, status)` | WARNING_HI | — | I2C read failed |

---

### HeatShutup

Manages JEITA temperature zone charging profiles and TS pin thermistor control.
Publishes thermal zone and TS voltage telemetry every `schedIn` tick.

**Has schedIn** — reads thermal zone and TS voltage every cycle. `ThermalZoneChanged` fires only when the zone actually changes, not every tick.

> **Note:** Temperature zone threshold configuration (`configure_TS_Tx_Charging_Threshold`, `reset_TS_lvl`) is available as direct C++ calls for use in `Topology.cpp` at startup, but is not exposed as a ground command.

#### Commands

| Command | Arguments | Register | Description |
|---|---|---|---|
| `HS_JEITA_ENABLE` | — | CHARGE_REGION_CONT (0x1C) bit 1 = 1 | Enable JEITA temperature zone profile |
| `HS_JEITA_DISABLE` | — | CHARGE_REGION_CONT bit 1 = 0 | Disable JEITA profile |
| `HS_TS_ENABLE` | — | CHARGE_REGION_CONT bit 0 = 1 | Enable TS pin thermistor function |
| `HS_TS_DISABLE` | — | CHARGE_REGION_CONT bit 0 = 0 | Disable TS pin thermistor |

#### Telemetry

| Channel | ID | Type | Description |
|---|---|---|---|
| `ThermalZone` | 0 | TS_LVL | Current JEITA thermal zone from CHARGER_STATUS_2 bits [6:4] |
| `TSVoltagePercent` | 1 | F32 | TS pin voltage as % of REGN. Formula: (TS_ADC[9:0] / 1024) × 100 |
| `JEITAEnabled` | 2 | bool | true if JEITA profile is active |
| `TSEnabled` | 3 | bool | true if TS pin function is active |

#### TS_LVL enum

| Value | Meaning |
|---|---|
| `TS_NORMAL` (0) | Temperature in normal range |
| `TS_WARM` (1) | Warm — reduced charge current |
| `TS_COOL` (2) | Cool — reduced charge current |
| `TS_COLD` (3) | Cold — charging suspended |
| `TS_HOT` (4) | Hot / T5 — charging suspended. BQ25756 skips T4. |
| `TS_INVALID` (5) | Unrecognized register value |

#### Events

| Event | Severity | Description |
|---|---|---|
| `JEITAEnabled` | ACTIVITY_LO | JEITA profile activated |
| `JEITADisabled` | ACTIVITY_LO | JEITA profile deactivated |
| `TSEnabled` | ACTIVITY_LO | TS pin thermistor activated |
| `TSDisabled` | ACTIVITY_LO | TS pin thermistor deactivated |
| `ThermalZoneChanged(zone)` | ACTIVITY_LO | Zone changed — fires on transition only, not every tick |
| `I2cReadFailed(reg, status)` | WARNING_HI | I2C read failed |
| `I2cWriteFailed(reg, status)` | WARNING_HI | I2C write failed |

---

### SafetyConfig

Controls charging enable/disable, MPPT, watchdog timer, and TS pin function.

**No schedIn** — these are configuration registers that change infrequently. Commands trigger actions; events confirm them.

#### Commands

| Command | Arguments | Register | Description |
|---|---|---|---|
| `SC_ENABLE_MPPT` | — | MPPT_CONT (0x1A) bit 0 = 1 | Enable MPPT. **When active, device controls ADC and writes to REG2A are ignored.** |
| `SC_DISABLE_MPPT` | — | MPPT_CONT bit 0 = 0 | Disable MPPT |
| `SC_INTERRUPT_CHARGING` | — | CHARGER_CONT (0x17) bit 0 = 0 | Stop charging immediately (clears EN_CHG) |
| `SC_ENABLE_CHARGING` | — | CHARGER_CONT bit 0 = 1 | Enable charging (sets EN_CHG) |
| `SC_DISABLE_WATCHDOG` | — | TIME_CONT (0x15) bits [5:4] = 00 | Disable watchdog timer |
| `SC_ENABLE_TS_PIN` | — | CHARGE_REGION_CONT (0x1C) bit 0 = 1 | Enable TS pin function for forward charging and reverse discharging |

#### Events

| Event | Severity | Description |
|---|---|---|
| `MPPTEnabled` | ACTIVITY_LO | MPPT activated — ADC now device-controlled |
| `MPPTDisabled` | ACTIVITY_LO | MPPT deactivated |
| `ChargingInterrupted` | WARNING_LO | Charging stopped by SC_INTERRUPT_CHARGING |
| `ChargingEnabled` | ACTIVITY_LO | Charging enabled by SC_ENABLE_CHARGING |
| `WatchdogDisabled` | ACTIVITY_LO | Watchdog timer disabled |
| `TSPinEnabled` | ACTIVITY_LO | TS pin function control enabled |
| `I2cReadFailed(reg, status)` | WARNING_HI | I2C read failed |
| `I2cWriteFailed(reg, status)` | WARNING_HI | I2C write failed |

---

## Topology Wiring

```fpp
connections BQ25756Connections {

    # All components share one LinuxI2cDriver instance
    adcControl.busWriteRead      -> linuxI2cDriver.writeRead
    adcControl.busWrite          -> linuxI2cDriver.write

    batteryMonitor.busWriteRead  -> linuxI2cDriver.writeRead
    batteryMonitor.busWrite      -> linuxI2cDriver.write

    faultStatus.busWriteRead     -> linuxI2cDriver.writeRead
    faultStatus.busWrite         -> linuxI2cDriver.write

    heatShutup.busWriteRead      -> linuxI2cDriver.writeRead
    heatShutup.busWrite          -> linuxI2cDriver.write

    safetyConfig.busWriteRead    -> linuxI2cDriver.writeRead
    safetyConfig.busWrite        -> linuxI2cDriver.write

    # Rate group wiring for periodic components (1 Hz example)
    rateGroup1Hz.RateGroupMemberOut[0] -> batteryMonitor.schedIn
    rateGroup1Hz.RateGroupMemberOut[1] -> faultStatus.schedIn
    rateGroup1Hz.RateGroupMemberOut[2] -> heatShutup.schedIn

    # Framework plumbing (repeat for each component)
    batteryMonitor.logOut     -> eventLogger.LogRecv
    batteryMonitor.tlmOut     -> tlmSend.TlmRecv
    batteryMonitor.timeGetOut -> linuxTime.getTime
    # ... same pattern for adcControl, faultStatus, heatShutup, safetyConfig
}
```

**Topology.cpp init:**
```cpp
// Open I2C bus — the only place /dev/i2c-1 appears in the entire codebase
linuxI2cDriver.open("/dev/i2c-1");

// Configure HeatShutup thresholds at startup (not exposed as commands)
I2cContext ctx = heatShutup.makeCtx();
heatShutup.reset_TS_lvl(ctx);  // restore factory defaults
```

---

## File Structure

```
BQ25756/
├── BQ25756Types.fpp            # Shared enums: ChargingStatus, TS_LVL, AdcChannel,
│                               # AdcRate, TS_Tx_prcnt, and BatteryMeasurements struct
├── BQ25756Ports.fpp            # Shared port type definitions
├── BQ25756.hpp                 # Top-level namespace forward declarations
│
├── i2c.hpp / i2c.cpp           # I2C helper free functions (read8bitRegister,
│                               # read16bitRegister, writeRegister) — no FPrime dependency
│
├── BQ25756_ADCControl.hpp      # ADCControl class declaration
├── BQ25756.cpp                 # ADCControl methods + command handlers + resetRegister
├── ADCControl.fpp              # ADCControl FPrime component definition
│
├── BatteryMonitor.hpp          # BatteryMonitor class declaration
├── BatteryMonitor.cpp          # BatteryMonitor implementation
├── BatteryMonitor.fpp          # BatteryMonitor FPrime component definition
│
├── FaultStatus.hpp             # FaultStatus class declaration
├── FaultStatus.cpp             # FaultStatus implementation
├── FaultStatus.fpp             # FaultStatus FPrime component definition
│
├── HeatShutup.hpp              # HeatShutup class declaration
├── HeatShutup.cpp              # HeatShutup implementation
├── HeatShutup.fpp              # HeatShutup FPrime component definition
│
├── SafetyConfiguration.hpp     # SafetyConfig class declaration
├── SafetyConfiguration.cpp     # SafetyConfig implementation
├── SafetyConfiguration.fpp     # SafetyConfig FPrime component definition
│
├── BQ25756_reg.h               # Register addresses and I2C_BUS_ADDR (0x6B)
└── CMakeLists.txt              # FPrime module build file
```