# TODO

Working list for `RuL-firmware`. Check items off as they land. See `AGENTS.md` for hardware and architecture.

Status today: board init + power latch work; `main()` then buzzes forever. Sensors, OLED, flash log, and real flight detection are **not running**.

---

## 1. Bring-up — make the intended loop run

- [ ] Replace the infinite `Buzzer_ON()` loop in `source/main.c` with the intended sequence: sensor init → flash/FS init → OLED init → flight FSM init → measure / detect / log / display.
- [ ] Call `LIS_init()`, `LPS_init()`, `FLASH_init()`, `OLED_init()`, `FlightState_Init(&sensors_d)` after `BOARD_Init*` / `PSU_turnOn()`.
- [ ] Sample in the main loop using `meas_period` / `log_period` (or `Clock`), not inside `SysTick_Handler`.
  - [ ] `LIS_update(&sensors_d)`
  - [ ] `LPS_update(&sensors_d)`
  - [ ] `sensors_d.time = Clock`
  - [ ] `FlightState_Detect(Clock)`
- [ ] Re-enable OLED from a non-ISR path: `OLED_render`, `OLED_nextScreen`, `OLED_sleepScreen` (today they are commented in SysTick).
- [ ] Remove dead locals in `main()` (`TPM_2_config`, `TPM_2_pwmSignalParams`, unused `freq`) — TPM is already started in `BOARD_InitBootPeripherals()`.
- [ ] Confirm WHO_AM_I / JEDEC on hardware: `LIS_WhoIam()` → `0x44`, `LPS_WhoIam()` → `0xBD`, `SPI_MemoryCheck()` → `01 60 17`.

## 2. Flight detection (placeholder today)

States exist (`WAIT_FOR_LAUNCH` → `ASCENT` → `FALLING` → `LANDING`) but every transition is `time - 3000 >= fsdEventTime && (1)`.

- [ ] Implement `FlightState_DelayInit` / `FlightState_DelayCheck` (declared, never defined).
- [ ] Launch: accel / pressure change, not a 3 s timeout.
- [ ] Apogee / falling: altitude or vertical accel, not 3 s.
- [ ] Landing: near-zero velocity / settled pressure, not 3 s.
- [ ] Track and store maxima in `max_d` (velocity, accel, altitude) — OLED screen 1 already displays them.
- [ ] Compute `sensors_d.altitude` from pressure (`LPS_update` currently writes `0.0f`). Velocity is never integrated.

Keep this logic in `flightStateDetector.c`, not SysTick.

## 3. Flash logging / filesystem

`FLASH_writeByte` / `readByte` / page helpers exist. Buffering, erase, and FS are empty.

- [ ] Implement `FLASH_init()` (mode, 4-byte addressing if needed, status check).
- [ ] Implement `FLASH_sectorErase()`.
- [ ] Implement `FLASH_push` / buffer / `FLASH_write` so `sensors_t` records (32 B) can be appended.
- [ ] Verify `PAGE_SIZE` vs transfer length: comment says log2 (n=8 → 256 B) but `2<<PAGE_SIZE` with `PAGE_SIZE 8` is 512. Fix before page R/W.
- [ ] Implement `FS_initFS`, `FS_getFilesFromTable`, `FS_getFilesFromPages` and declare them in `FS.h` (only `FS_testWR` is exported).
- [ ] Define log layout: start address, record size, wrap/erase policy, how “file” boundaries work.
- [ ] Drive `status_d.memfree` from real free space (ADC ISR currently hardcodes `100.0f`).
- [ ] Add a safe erase path (button combo / debug build) so a full chip is not erased by accident.

## 4. OLED / UI

- [ ] Restore short-press → `OLED_nextScreen()` and 5 s idle → `OLED_sleepScreen()` **from the main loop**, not SysTick.
- [ ] Finish `OLED_turnOff()` low-power path (comment: “Dopisać wyłącznie wyświetlacza LP”).
- [ ] Confirm 96×32 init vs multiplex comment (`0xA8` / `31` vs leftover “1/64” comment).
- [ ] Decide whether `dev_CheckSensors()` stays as a bring-up screen (it still mentions BMP/ADXL, not LIS/LPS).

## 5. Bugs / correctness

- [ ] `I2C_Read`: `dataSize = length-1` with `????` — confirm against NXP DMA API; likely off-by-one.
- [ ] `LIS_readRegN` / `LPS_readRegN`: TX command lives in local `buf`, RX is `val` — DMA full-duplex will not put the command byte on MOSI if `txData` is `val`. Fix or stop using these helpers.
- [ ] `delay()` and SPI/I2C waits must not run from ISRs. Move remaining SysTick work (button debounce is already there) toward flags + main loop.
- [ ] Button: document / verify polarity. `Button_state()` is raw PTA1; hold-2 s power-off assumes pressed = 1.
- [ ] Align Config Tools YAML vs live C where they disagree (I2C 100 kHz YAML vs 600 kHz C; ADC YAML 12-bit vs C 16-bit). Either regenerate from `.mex` or note that C is source of truth.

## 6. Power

- [ ] Implement `PSU_lowPowerMode()` / `PSU_highPowerMode()` (empty).
- [ ] After landing: dim/off OLED, stop logging, keep power latch until 2 s hold.
- [ ] Battery: confirm 1:2 divider and 3.0 V Vref on this PCB; ADC Config Tools vs `analog.c` formula.

## 7. Tooling / hygiene

- [ ] Update `script.jlink` load path (still `I_PRV_RuL_1_1.hex` on another machine).
- [ ] Drop unused MCUXpresso absolute include paths from `.cproject` if the project is moved.
- [ ] Fill `README.md` (title only today) or keep it a pointer to `AGENTS.md`.
- [ ] Optional: CMake / Makefile for CI size/build without MCUXpresso (not in repo now).

---

## Suggested order

1. WHO_AM_I + flash JEDEC + OLED hello (prove buses).
2. Wire measure loop + OLED status screen.
3. Real altitude / maxima, then flight FSM.
4. Flash append log + `memfree`.
5. Power modes and landing behavior.
