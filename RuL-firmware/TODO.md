# TODO

Working list for `RuL-firmware`. Check items off as they land. See `AGENTS.md` for hardware and architecture.

Status today: `main()` inits LPS, LIS, OLED, flash CS, and the flight FSM, probes WHO_AM_I / JEDEC, then samples sensors on `meas_period`. OLED screens and button (next / sleep / 2 s power-off) run from SysTick. Buzzer is a 1 ms service (`Buzzer_shortBeep` / `pulse` / `stop`). **Flash log is still commented. Flight transitions are still 3 s placeholders.** One short beep = bus OK; three beeps + red LED = ID fail.

---

## 1. Bring-up — make the intended loop run

- [x] Replace the infinite `Buzzer_ON()` loop in `source/main.c` with init + measure / detect / (log) / display.
- [x] Call `LIS_init()`, `LPS_init()`, `OLED_init()`, `FlightState_Init(&sensors_d)` after `BOARD_Init*` / `PSU_turnOn()`.
- [x] Call `FLASH_init()` (and FS init once it exists).
- [x] Sample in the main loop using `meas_period` / `log_period`, not inside `SysTick_Handler`.
  - [x] `LIS_update(&sensors_d)`
  - [x] `LPS_update(&sensors_d)`
  - [x] `sensors_d.time = Clock`
  - [x] `FlightState_Detect(Clock)`
- [x] OLED live: `OLED_render`, `OLED_nextScreen`, `OLED_sleepScreen` (still in SysTick — move out, see §4 / §5).
- [x] Remove dead locals in `main()` (`TPM_2_config`, `TPM_2_pwmSignalParams`, unused `freq`).
- [x] Stop calling `Buzzer_ON()` + `delay(300)` every loop iteration (blocks sampling; beep should be event-based).
- [x] Confirm WHO_AM_I / JEDEC on hardware: `LIS_WhoIam()` → `0x44`, `LPS_WhoIam()` → `0xBD`, `SPI_MemoryCheck()` → `01 60 17`. (boot: 1 beep = OK, 3 beeps + red LED = fail)

## 2. Flight detection (placeholder today)

States exist (`WAIT_FOR_LAUNCH` → `ASCENT` → `FALLING` → `LANDING`) but every transition is `time - 3000 >= fsdEventTime && (1)`.

`main()` already changes sample/log rates by state:

| State | meas | log |
|---|---|---|
| WAIT_FOR_LAUNCH | 100 ms | 500 ms |
| ASCENT | 100 ms | 100 ms |
| FALLING | 500 ms | 500 ms |
| LANDING | 500 ms | 1000 ms |

- [ ] Implement `FlightState_DelayInit` / `FlightState_DelayCheck` (declared, never defined).
- [ ] Launch: accel / pressure change, not a 3 s timeout.
- [ ] Apogee / falling: altitude or vertical accel, not 3 s.
- [ ] Landing: near-zero velocity / settled pressure, not 3 s.
- [ ] Track and store maxima in `max_d` (zeroed at boot; OLED screen 1 already displays them).
- [ ] Compute `sensors_d.altitude` from pressure (`LPS_update` currently writes `0.0f`). Velocity is never integrated.

Keep this logic in `flightStateDetector.c`, not SysTick.

## 3. Flash logging / filesystem

`FLASH_writeByte` / `readByte` / page helpers exist. Buffering, erase, and FS are empty. Main loop log slot is `//FLASH_write(&sensors_d);`.

- [ ] Implement `FLASH_init()` body (4-byte addressing if needed, wait-ready with timeout). Today it only deasserts CS and leaves sensor SPI mux.
- [ ] Implement `FLASH_sectorErase()`.
- [ ] Implement `FLASH_push` / buffer / `FLASH_write` so `sensors_t` records (32 B) can be appended.
- [ ] Uncomment / wire the `counter_log` path in `main()`.
- [ ] Verify `PAGE_SIZE` vs transfer length: comment says log2 (n=8 → 256 B) but `2<<PAGE_SIZE` with `PAGE_SIZE 8` is 512. Fix before page R/W.
- [ ] Implement `FS_initFS`, `FS_getFilesFromTable`, `FS_getFilesFromPages` and declare them in `FS.h` (only `FS_testWR` is exported).
- [ ] Define log layout: start address, record size, wrap/erase policy, how “file” boundaries work.
- [ ] Drive `status_d.memfree` from real free space (ADC ISR currently hardcodes `100.0f`).
- [ ] Add a safe erase path (button combo / debug build) so a full chip is not erased by accident.

## 4. OLED / UI

- [ ] Move short-press → `OLED_nextScreen()`, 5 s idle → `OLED_sleepScreen()`, and 250 ms `OLED_render` **out of SysTick** into the main loop (flag/`Clock` gated). Blocking I2C from the ISR stalls the tick.
- [ ] Finish `OLED_turnOff()` low-power path (comment: “Dopisać wyłącznie wyświetlacza LP”).
- [ ] Confirm 96×32 init vs multiplex comment (`0xA8` / `31` vs leftover “1/64” comment).
- [ ] Decide whether `dev_CheckSensors()` stays as a bring-up screen (it still mentions BMP/ADXL, not LIS/LPS).

## 5. Bugs / correctness

- [ ] `I2C_Read`: `dataSize = length-1` with `????` — confirm against NXP DMA API; likely off-by-one.
- [ ] `LIS_readRegN` / `LPS_readRegN`: TX command lives in local `buf`, RX is `val` — DMA full-duplex will not put the command byte on MOSI if `txData` is `val`. Fix or stop using these helpers.
- [ ] `delay()` and SPI/I2C waits must not run from ISRs. SysTick currently does OLED I2C and `while(Button_state())` on power-off.
- [ ] Button: document / verify polarity. `Button_state()` is raw PTA1; hold-2 s power-off assumes pressed = 1.
- [ ] Align Config Tools YAML vs live C where they disagree (I2C 100 kHz YAML vs 600 kHz C; ADC YAML 12-bit vs C 16-bit). Either regenerate from `.mex` or note that C is source of truth.
- [ ] Boot `printf` vs `SDK_DEBUGCONSOLE=0` — confirm where (if anywhere) those strings go.

## 6. Power

- [ ] Implement `PSU_lowPowerMode()` / `PSU_highPowerMode()` (empty).
- [ ] After landing: dim/off OLED, stop logging, keep power latch until 2 s hold.
- [ ] Battery: confirm 1:2 divider and 3.0 V Vref on this PCB; ADC Config Tools vs `analog.c` formula.

## 7. Tooling / hygiene

- [ ] Update `script.jlink` load path (still `I_PRV_RuL_1_1.hex` on another machine).
- [ ] Drop unused MCUXpresso absolute include paths from `.cproject` if the project is moved.
- [x] Fill `README.md` (keep it in sync with `main.c`).
- [ ] Optional: CMake / Makefile for CI size/build without MCUXpresso (not in repo now).

---

## Suggested order

1. On hardware: 1 beep vs 3 beeps + red LED (WHO_AM_I / JEDEC).
2. Move OLED work out of SysTick.
3. Real altitude / maxima, then flight FSM (periods in `main()` already wait on state).
4. Flash append log + `memfree` (`FLASH_init` is CS/mode only; write path still commented).
5. Power modes and landing behavior.
