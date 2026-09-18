# RuL-firmware

Bare-metal firmware for the **I_PRV_RuL_1_1** amateur-rocket flight computer.

MCU: NXP **MKL27Z256VFM4** (Cortex-M0+, 8 MHz). No RTOS. Built with MCUXpresso / NXP SDK 2.x.

## What it does

- Samples accelerometer (LIS2HH12) and barometer (LPS25HB)
- Detects flight phases (launch → ascent → falling → landing)
- Logs records to SPI NOR flash (S25FL064L)
- Shows status on a 96×32 OLED
- Power latch, RGB LEDs, buzzer, battery ADC

**Current tree is incomplete:** `main()` only holds power and buzzes. Sensors, OLED, logging, and real flight detection are not wired yet. See [TODO.md](TODO.md).

## Layout

| Path | Contents |
|---|---|
| `source/` | Application code (edit here) |
| `board/` | Pins, clocks, peripherals (MCUXpresso Config Tools) |
| `drivers/` | NXP SDK drivers |
| `device/`, `CMSIS/`, `startup/` | MCU support — leave alone |

## Build and flash

1. Open this folder as a project in **MCUXpresso**.
2. Build Debug or Release (`RuL-firmware.axf`).
3. Flash over **SWD** (J-Link). `script.jlink` is a starting point; the hex path inside is outdated.

There is no CMake/Makefile in the repo. `Debug/` and `Release/` are gitignored.

## Docs

- [AGENTS.md](AGENTS.md) — hardware map, architecture, coding rules
- [TODO.md](TODO.md) — remaining work
