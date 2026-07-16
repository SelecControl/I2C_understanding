# I2C Understanding — Project Status & Handoff

**Project:** Learning I2C from first principles using an ESP32 + OLED display, then building a working OLED program piece by piece.
**Format inspired by:** [claude_meter](https://github.com/rohanshinde1234/claude_meter) — this README follows the same "narrative handoff" style so a new chat (e.g., on another PC) can pick up without re-explaining everything.
**Purpose of this document:** Continuity reference. Claude Code conversation history is **local to the machine + folder path it was created in** (no cloud sync) — this file is the actual continuity mechanism across machines, not the chat transcript.
**Status (as of 2026-07-16):** All six original learning goals are complete (see section 4). Both `oled_test` and `i2c_scanner` have been compiled, flashed, and verified working on real hardware from the home PC (section 3.1 toolchain) — I2C scanner correctly detected the OLED's port/address, and "Hello ESP32!" displayed properly on screen. Project's core goals are fully done end-to-end.

---

## 1. How This Project Is Meant to Be Taught (Read First, Important)

This is a **guided learning session**, not just a build log. If you're a fresh Claude session picking this up, follow these rules exactly — they were set explicitly by the user:

- Cover only **one concept at a time**.
- Explain **theory first**, then show a **small code snippet** related only to that concept — never the full program upfront.
- Explain every new term in simple language, using analogies/ASCII diagrams where helpful.
- After each concept, ask a couple of small questions or an exercise to check understanding.
- If the user makes a mistake, give **hints**, not the direct answer.
- **Do not move to the next topic until the user explicitly says "Next."** Answering the questions is not the same as saying "Next" — wait for the explicit word.
- Assume the user is a beginner. Be patient.

## 2. Hardware in Use

| Component | Identification | Notes |
|---|---|---|
| Microcontroller | ESP32 (see `Images/ESP32.png`) | |
| Display | 1.3" I2C OLED, **SH1106 driver**, 128×64 (see `Images/Oled.png`) | Confirmed by user — **not** SSD1306, despite visual similarity to common 0.96" modules. Must use `Adafruit_SH110X` library / `Adafruit_SH1106G` class, not `Adafruit_SSD1306` |

### 2.1 Wiring (confirmed working — see `Images/PIN_connections.png`)

| ESP32 pin | Connects to | Purpose |
|---|---|---|
| 3V3 | OLED VCC | Power |
| GND | OLED GND | Ground |
| GPIO22 | OLED SCK (SCL) | I2C clock |
| GPIO21 | OLED SDA | I2C data |
| GPIO27 | Button leg 1 | Digital input, `INPUT_PULLUP` (not part of I2C learning — noted for later) |
| GND | Button leg 2 | Ground |

These are also the ESP32's **default** `Wire` library pins (SDA=21, SCL=22), so no custom pin arguments are needed in `Wire.begin()`.

### 2.2 Project folder structure (current)

```
I2C_understanding/
  README.md                 <- this file
  Images/                   <- ESP32.png, Oled.png, PIN_connections.png
  oled_test/
    oled_test.ino            <- complete "Hello ESP32!" program, written and explained; not yet compiled/flashed
  i2c_scanner/
    i2c_scanner.ino          <- standalone I2C address-scanning/debugging sketch; not yet compiled/flashed
```

## 3. Toolchain (Decided)

Matches the reference project's philosophy: **no vendor-specific IDE.**

- **arduino-cli** — not the Arduino IDE GUI, not PlatformIO. VS Code is used purely as a text editor; compiling/flashing happens via CLI.
- Library stack (planned): `Wire.h` (built-in, I2C communication) + `Adafruit_SH110X` + `Adafruit_GFX` (+ `Adafruit_BusIO` auto-dependency).
- Toolchain setup/installation itself is **out of scope for the office PC session** — the user handles arduino-cli install/board config on their own machine(s). Don't attempt to install or verify it via shell commands.

### 3.1 Home PC — Confirmed Installed (as of 2026-07-16)

Verified via `arduino-cli core list` / `arduino-cli lib list` on this machine:

| Item | Version |
|---|---|
| `arduino-cli` | 1.5.1 (`C:\Users\SAKSHI\arduino-cli\arduino-cli.exe`) |
| Core `esp32:esp32` | 3.3.10 |
| Library: Adafruit BusIO | 1.17.4 |
| Library: Adafruit GFX Library | 1.12.6 |
| Library: Adafruit SH110X | 2.1.14 |
| Library: Adafruit SSD1306 | 2.5.17 (extra — not used, harmless) |

### 3.2 Office PC — Learning/Writing Only, No Toolchain Install (decided 2026-07-16)

Office PC has internet, but **no admin rights** (confirmed via a Device Manager dialog restricting device changes to administrators). `pnputil /enum-drivers` (runnable without admin) confirmed no CP210x/Silicon Labs USB-to-UART driver is staged on this machine — so flashing here would likely hit a driver wall needing admin rights this account doesn't have.

**Decision:** Don't install `arduino-cli`/core/libraries on the office PC at all — not worth the time/effort given flashing has to happen elsewhere regardless. Split cleanly instead:
- **Office PC**: learning concepts, reading/writing/understanding code only (this is what `oled_test.ino` and `i2c_scanner.ino` are for).
- **Home PC**: all actual compiling and flashing, using the toolchain already confirmed working there (section 3.1).

## 4. Progress Log

### All six original learning goals: complete ✅

- [x] **What I2C is and why it's used** — 2-wire shared bus vs. dedicated wires per device; controller (ESP32) vs. peripheral (OLED) terminology
- [x] **SDA and SCL** — SCL as clock/timing ("drumbeat"), SDA as the actual data line, data read on clock edge
- [x] **Pull-up resistors** — open-drain wires (devices can only pull LOW or release), resistor pulls the line HIGH by default; OLED breakout likely has these built in
- [x] **Device addresses** — 7-bit address (e.g., OLED = `0x3C`) so multiple devices can share the same 2 wires; hex notation (`0x..`) intro; `#define` intro; address is fixed by the manufacturer in the chip (not set via jumper, beyond an optional 1-2 bit tweak on some boards)
- [x] **What the `Wire` library does** — abstracts away manual clock/data bit-banging; `#include <Wire.h>`, `setup()` as one-time init function, `Wire.begin()` defaults to GPIO21/22 on ESP32
- [x] **`Wire.beginTransmission()` / `Wire.endTransmission()`** — starting/ending an I2C conversation with a specific address; ACK vs NACK on the SDA line (9th clock pulse); `endTransmission()` returns a status code — 0 (ACK/success) or non-zero (NACK/failure) — sends the STOP condition, and releases the bus, no auto-retry
- [x] **`Adafruit_GFX` + `Adafruit_SH110X`** — split responsibility: `GFX` draws into an in-memory buffer (hardware-agnostic "artist"), `SH110X` delivers that buffer over I2C via `Wire` to the specific SH1106 chip (the "courier"); class vs. object; constructor args in `Adafruit_SH1106G display(128, 64, &Wire, -1)` (width, height, I2C bus, no hardware reset pin → software reset instead)
- [x] **`display.begin(0x3C, true)`** — one-time chip init (internal voltages, memory, reset); returns `true`/`false` based on whether the device ACKed at that address — same ACK/NACK mechanism as `endTransmission()`, just interpreted internally
- [x] **Buffer + `display.display()`** — all `Adafruit_GFX` drawing calls (`clearDisplay`, `setTextSize`, `setTextColor`, `setCursor`, `println`) only edit the invisible in-memory buffer; nothing appears on the physical screen until `display.display()` pushes the whole frame at once (avoids visible partial/flickering draws)
- [x] **Complete working program assembled** — `setup()`/`loop()` explained; full "Hello ESP32!" sketch traced line-by-line end to end, including why `while(true);` must sit *inside* the `if (!display.begin(...))` block (placing it outside would freeze the program every run, even on success) — written to `oled_test/oled_test.ino`
- [x] **I2C scanner, fully written and explained** — loops `beginTransmission()`/`endTransmission()` over addresses 1–126, checks for return `0` (ACK); introduces `Serial` (USB debug output, separate from I2C), `for` loops, and `delay()`; written to `i2c_scanner/i2c_scanner.ino`
- [x] **Debugging with the scanner** — reasoned through real failure scenarios: no devices found (wiring/pull-ups/power), device at unexpected address (update code), swapped SDA/SCL (breaks ACK entirely since clock and data roles are no longer distinguishable)

### Decisions made along the way

- Initially considered raw ESP-IDF I2C driver (no Arduino library) — **superseded**. Final decision: match the reference repo exactly — arduino-cli + Wire.h + Adafruit libraries (see section 3).
- Confirmed OLED driver chip: **SH1106**, not SSD1306 (see section 2).

### Compiled and flashed — confirmed working on hardware ✅

- [x] **`i2c_scanner.ino`** — compiled with `arduino-cli compile --fqbn esp32:esp32:esp32 i2c_scanner`, uploaded to COM3, correctly detected the OLED on the bus
- [x] **`oled_test.ino`** — compiled and uploaded the same way; "Hello ESP32!" displayed correctly on the physical OLED

### Not yet done

- Nothing outstanding from the original learning goals. See "Optional deeper I2C topics" below if you want to keep going beyond the original scope.

### Optional deeper I2C topics (not covered — outside the original goals, available on request if ever needed)

- Clock stretching (peripheral pausing SCL when it needs more time)
- Read operations (we only ever wrote to the OLED; reading involves the address's R/W bit and NACK/ACK during reads)
- Repeated START / combined write-then-read transactions
- Multi-master arbitration (only relevant with more than one controller on the bus)
- 10-bit addressing (beyond the 7-bit addressing used here)
- Speed modes (Standard 100kHz used by default here, vs. Fast/Fast Mode+/High Speed)

## 5. How to Resume This Session Elsewhere

1. Clone/copy this project folder (including `Images/` and this `README.md`).
2. Start a new Claude Code session in that folder.
3. Point Claude at this README and say something like: *"Continue teaching me I2C from where README.md's progress log leaves off, following the same teaching rules in section 1."*
4. No need to re-explain hardware, wiring, toolchain decisions, or concepts already covered — they're all captured above.
5. **Toolchain installation is deferred everywhere until core understanding is done** — see section 3 for what's already confirmed working on the home PC. Both the home and office PCs have internet, so a fresh install is fine wherever/whenever it happens — no USB transfer needed.
