# RuL-firmware

Bare-metal firmware for the **I_PRV_RuL_1_1** amateur-rocket flight computer.

MCU: NXP **MKL27Z256VFM4** (Cortex-M0+, 8 MHz). No RTOS. Built with MCUXpresso / NXP SDK 2.x.

## What it does

- Samples accelerometer (LIS2HH12) and barometer (LPS25HB)
- Detects flight phases (launch → ascent → falling → landing) — **3 s placeholders**, not real flight logic
- Logs records to SPI NOR flash (S25FL064L) — **write path still commented**
- Shows status on a 96×32 OLED (button cycles screens; 5 s idle sleeps)
- Power latch, RGB LEDs, buzzer, battery ADC

Boot: power latch → LPS / LIS / OLED / flash CS → WHO_AM_I + JEDEC check → flight FSM → measure loop. Sample and log rates depend on flight state. Flash writes are not enabled. One short beep at boot means the buses answered; three beeps and a red LED means an ID failed (`Buzzer_service` in SysTick). See [TODO.md](TODO.md).

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
