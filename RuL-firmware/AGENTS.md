# RuL-firmware — AI working notes

Bare-metal firmware for a custom amateur-rocket flight computer (`I_PRV_RuL_1_1`). MCUXpresso / NXP SDK 2.x project. No RTOS.

Use this file as the first context for any change. Prefer application code in `source/` over generated SDK / Config Tools files.

## What this board is

| Item | Value |
|---|---|
| MCU | NXP **MKL27Z256VFM4** (Cortex-M0+, 256 KB flash, QFN32) |
| Core clock | **8 MHz** (`BOARD_BOOTCLOCKRUN_CORE_CLOCK`) |
| OS | Bare metal (`SDK_OS_BAREMETAL`, `FSL_RTOS_BM`) |
| IDE / toolchain | MCUXpresso (originally 10.3.1), `arm-none-eabi-gcc`, **Redlib**, C11, Thumb |
| Debug | SWD (`PTA0` CLK, `PTA3` DIO) |
| Config Tools | `RuL-firmware.mex` — pins / clocks / peripherals |

Purpose: sample IMU + barometer, detect flight phases, log to SPI NOR flash, show status on a small OLED, hold power with a GPIO latch, beep via PWM.

## Directory map

```
source/          Application — edit here
board/           MCUXpresso Config Tools output (pins, clocks, peripherals)
drivers/         NXP fsl_* SDK drivers — do not fork unless unavoidable
device/          CMSIS device headers for MKL27Z4
CMSIS/           ARM CMSIS core
startup/         Vector table / reset (startup_mkl27z4.c)
```

| File | Role |
|---|---|
| `source/main.c` | Boot, SysTick (button + OLED), ADC ISR, measure loop + flight FSM; flash log commented |
| `source/struct.h` | Shared types: `sensors_t`, `stat_t`, `max_t` |
| `source/utils.c` | Blocking `delay()`, RGB LEDs, button, power GPIO |
| `source/spi.c` | SPI0 DMA master + chip-selects + MOSI/MISO remap for flash |
| `source/i2c.c` | I2C0 DMA master (OLED) |
| `source/LIS.c` | LIS2HH12 accelerometer (SPI) |
| `source/LPS25HB.c` | LPS25HB barometer + temperature (SPI) |
| `source/flash.c` | S25FL064L 4-byte-address NOR (SPI) |
| `source/FS.c` | Intended filesystem/log layer — **stubs** |
| `source/oled.c` | 96×32 SSD1306-style OLED over I2C 0x3C |
| `source/buzzer.c` | TPM2 PWM buzzer **service** (`Buzzer_service` every 1 ms) |
| `source/analog.c` | Battery voltage from ADC raw |
| `source/PSU.c` | Power latch on / off |
| `source/flightStateDetector.c` | Flight-phase FSM — **placeholder 3 s timeouts** |
| `source/fonts.h` | OLED glyph table |

## Hardware map

Chip-select helpers are **active-low** but the API is inverted: `SPI_CS_*(1)` asserts CS (drives pin 0).

LEDs are **active-low**: `LED_*(1)` turns the LED on.

| Function | Pin | Peripheral / notes |
|---|---|---|
| LED red | PTD5 | GPIO, active-low |
| LED blue | PTD6 | GPIO, active-low |
| LED green | PTD7 | GPIO, active-low |
| CS LIS (accel) | PTC3 | GPIO |
| CS LPS (baro) | PTC4 | GPIO |
| CS FLASH | PTD4 | GPIO |
| SPI0 SCK | PTC5 | SPI0 |
| SPI0 MOSI/MISO | PTC6 / PTC7 | Sensors: Alt2 (MOSI/MISO). Flash: swapped to Alt5 — see `SPI_SPI_FLASHMODE()` |
| I2C0 SCL / SDA | PTB0 / PTB1 | OLED |
| VBAT | PTE30 | ADC0 SE23, 1:2 divider, 3.0 V Vref assumed |
| Button | PTA1 | GPIO input, pull-up. `Button_state()` is raw pin (1 = pressed if active-high wiring) |
| Buzzer | PTA2 | TPM2_CH1 PWM, ~2800 Hz |
| OLED reset | PTA19 | GPIO |
| Power latch | PTE0 | GPIO. `PSU_turnOn()` keeps board powered; `PSU_turnOff()` cuts it |
| SWD | PTA0 / PTA3 | Debug |

### External parts

- **LIS2HH12** accel, SPI, WHO_AM_I `0x44`. Init: 25 Hz, ±16 g, 14-bit. Accel scale in code: `/ 2048.0f` → g.
- **LPS25HB** baro, SPI, WHO_AM_I `0xBD`. Init: 25 Hz, avg 8/8. Pressure `/40.960` → Pa. Temp `42.5 + raw/480` → °C. `altitude` is written as `0` (not computed).
- **S25FL064L** NOR flash, JEDEC `01 60 17`. Commands use 4-byte address (`PP4`/`READ4`). Page size helper: `PAGE_SIZE` is **log2**, so `n=8` → 256 B (`2<<PAGE_SIZE` in transfers looks wrong vs the comment — verify before using page R/W).
- **OLED** I2C address `0x3C`, 96×32, page addressing. Buffer `OLED_dispBuff[96][4]` (column, page). `OLED_refresh()` is **4 I2C writes** (page+column setup + 96 px per page).

### Peripherals (as configured)

- SPI0 master, **500 kHz**, 8-bit, mode 0, DMA ch 0/1, SS as GPIO.
- I2C0 master, **600 kHz** in C (`I2C_0_config`); Config Tools YAML still says 100 kHz — C wins until regenerated.
- TPM2 PWM 2800 Hz, ch1 duty 0 at init. `Buzzer_service()` in SysTick drives patterns; `Buzzer_shortBeep` / `longBeep` / `pulse` / `stop`. Duty 18% while on.
- ADC0: 16-bit SE, continuous, HW avg 32, IRQ on conversion. `Analog_setVbatRaw()` uses `2 * raw * 3 / 65536` (divider + 3 V ref).
- SysTick: 1 ms. `Clock` and `delay()` both depend on `SysTick_Handler` → `TimingDelay_Decrement()`.

## Runtime architecture

```
SysTick 1 ms
  ├─ delay() countdown
  ├─ Buzzer_service()  (tone / gap / stop)
  ├─ Clock++, counter_meas, counter_log
  ├─ every 100 ms: button hold 2 s → OLED clear + PSU_turnOff();
  │                 short press ≥300 ms → OLED_nextScreen()
  ├─ idle >5 s → OLED_sleepScreen()
  ├─ every 250 ms → OLED_render(&status_d, &sensors_d, &max_d)
  └─ heartbeat green LED (~3 s double blink)

ADC0 IRQ → Analog_setVbatRaw → status_d.vbat
           status_d.memfree currently hardcoded 100.0f

main():
  PSU_turnOn → LPS_init → LIS_init → OLED_init → FLASH_init → FS_initFS
  WHO_AM_I / JEDEC (LPS 0xBD, LIS 0x44, flash 01 60 17)
    OK → Buzzer_shortBeep(); fail → red LED + Buzzer_pulse(3, 80, 80, 0)
  FlightState_Init, OLED status template
  loop:
    set meas_period / log_period from FlightState_getState()
    if counter_meas elapsed: sensors_d.time = Clock; LPS_update; LIS_update; FlightState_Detect(Clock)
    if counter_log elapsed: FLASH_write commented out
```

State-dependent periods (ms, matching SysTick 1 ms counters):

| State | `meas_period` | `log_period` |
|---|---|---|
| WAIT_FOR_LAUNCH | 100 | 500 |
| ASCENT | 100 | 100 |
| FALLING | 500 | 500 |
| LANDING | 500 | 1000 |

OLED and button handling run in `SysTick_Handler`. I2C/SPI helpers are blocking DMA polls — **not ISR-safe**. `OLED_render` / `OLED_nextScreen` / power-off wait-for-release from SysTick can stall the 1 ms tick.

`FLASH_init()` only deasserts flash CS and restores sensor SPI mux. `sensors_d.time` is set on each measure. `max_d` is zeroed at boot and never updated.

### Shared data (`struct.h`)

`sensors_t` is a **32-byte union** meant for flash records:

- `time`, `acc` (`vector_t` xyz float), `vel`, `pressure`, `altitude`, `temp`
- overlay: `uint8_t array[32]`

Globals in `main.c`: `sensors_d`, `status_d`, `max_d`.

### Flight state machine

States: `WAIT_FOR_LAUNCH` → `ASCENT` → `FALLING` → `LANDING`.

Each state has a one-shot `state_ready` init, then a transition. **Current transitions are `time - 3000 >= fsdEventTime && (1)`** — 3 s placeholders, sensors unused. `FlightState_DelayInit` / `DelayCheck` are declared and **not implemented**.

### OLED screens

0: status (Vbat, pressure, flash free %)  
1: max meas (velocity, acc, altitude)  
2: sleep / off  

Short button press calls `OLED_nextScreen()` from SysTick.

## Current completeness (do not assume it flies)

`main()` latches power, inits LPS / LIS / OLED / flash CS / FS stub, probes WHO_AM_I + JEDEC, then samples sensors and runs `FlightState_Detect`. OLED UI is live in SysTick. **`FLASH_write` is commented.** Flight transitions are still 3 s placeholders. Buzzer is a 1 ms SysTick service: boot uses `Buzzer_shortBeep()` (OK) or `Buzzer_pulse(3, …)` (ID fail).

Boot `printf` traces exist (`MCU ready`, `LPS/LIS/OLED ready`). `SDK_DEBUGCONSOLE=0` — they only appear if semihosting / another console is actually wired.

Incomplete or suspicious:

- `FS.c` — empty `FS_initFS` (now called from `main`), table/page scanners; only `FS_testWR`.
- `flash.c` — `FLASH_init` is CS/mux only. `FLASH_sectorErase`, `FLASH_push`, buffer helpers empty. `FLASH_write(sensors_t*)` commented in both `flash.c` and the main log slot.
- OLED I2C from `SysTick_Handler` (render / next screen / clear on power-off).
- `PSU_lowPowerMode` / `highPowerMode` empty.
- `I2C_Read` uses `dataSize = length-1` with a `????` comment — treat as a bug until proven.
- `LPS_readRegN` / `LIS_readRegN` pass `val` as RX buffer but TX command is in a local `buf` — multi-byte reads may be wrong.
- `delay()` busy-waits; **never call from an ISR**. SPI/I2C helpers also spin on DMA complete flags — not ISR-safe.
- `script.jlink` still points at an old absolute path (`I_PRV_RuL_1_1.hex`).

## Coding conventions

- C, not C++. Pair `foo.c` / `foo.h` per driver. Include guard `FOO_H_`.
- NXP style mixed with local helpers. Prefix by module: `LIS_`, `LPS_`, `FLASH_`, `OLED_`, `PSU_`, `FlightState_`, `SPI_`, `I2C_`, `Analog_`, `Buzzer_`.
- Comments are **Polish or English**; keep the language of nearby comments. Do not mass-translate.
- Blocking DMA + flag poll is the existing I/O pattern. Do not introduce an RTOS or HAL rewrite unless asked.
- Active-low LEDs and inverted CS helpers: copy existing helpers, do not drive those pins “intuitively”.
- `__IO` used for delay tick (CMSIS).

## Files you must not casually edit

Regenerating MCUXpresso Config Tools **overwrites**:

- `board/pin_mux.c` / `.h`
- `board/clock_config.c` / `.h`
- `board/peripherals.c` / `.h`

If a pin or peripheral must change, prefer updating `RuL-firmware.mex` **or** document that you hand-edited generated files. Do not “fix” YAML comments vs C mismatches without checking which is live.

Do not rewrite `drivers/`, `device/`, `CMSIS/`, `startup/` for feature work.

## Build / flash

- Open `RuL-firmware` in MCUXpresso; Eclipse project files: `.project`, `.cproject`.
- Artifact: `RuL-firmware.axf` (Debug/Release). Post-build: `arm-none-eabi-size`.
- Defines (Debug): `CPU_MKL27Z256VFM4`, `__MCUXPRESSO`, `__USE_CMSIS`, `__REDLIB__`, `SDK_DEBUGCONSOLE=0`, `PRINTF_FLOAT_ENABLE=1`.
- Include paths: `source`, `board`, `drivers`, `device`, `CMSIS`.
- J-Link script: `script.jlink` (path inside is stale).

There is no CMake / Makefile checked in; the Eclipse managed builder generates `Debug/` / `Release/` (gitignored).

## Rules for AI changes

1. Smallest change that matches existing module style. New sensors or buses get their own `source/` pair, not a dump into `main.c`.
2. Keep `sensors_t` 32 bytes if flash logging will use the union overlay.
3. Extend the existing main loop (state periods → measure → log) rather than inventing a second architecture, unless the task is a redesign.
4. Flight detection belongs in `flightStateDetector.c`, not in `SysTick_Handler`.
5. Do not busy-wait in ISRs. SysTick already drives OLED over blocking I2C; move render / next-screen / sleep to the main loop with flags. `meas_period` / `log_period` already gate sensor work.
6. Test thoughts: WHO_AM_I (`LIS_WhoIam` 0x44, `LPS_WhoIam` 0xBD), `SPI_MemoryCheck()`, `FS_testWR()`, OLED templates, power latch, button timings (100 ms tick, 20 ticks ≈ 2 s hold).
7. Do not commit secrets, local MCUXpresso absolute include paths, or generated `Debug/`/`Release/` trees.

## Quick “where do I…?”

| Task | Where |
|---|---|
| Change boot / loop / button | `source/main.c` |
| Add a logged field | `struct.h` (`sensors_t`) + writers + OLED if displayed |
| Accel / baro init or scale | `LIS.c` / `LPS25HB.c` |
| Flash command / addressing | `flash.c` / `flash.h` |
| Log format / files | `FS.c` (needs implementing) |
| Display layout | `oled.c` templates + `OLED_render*` |
| Pinout | `board/pin_mux.*` + this table |
| PWM beep | `buzzer.c` — `Buzzer_shortBeep` / `longBeep` / `pulse` / `stop`; `Buzzer_service` in SysTick |
| Battery scale | `analog.c` |
