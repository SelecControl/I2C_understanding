# I2C Understanding — Project Status & Handoff

**Project:** Learning I2C from first principles using an ESP32 + OLED display, then building a working OLED program piece by piece.
**Format inspired by:** [claude_meter](https://github.com/rohanshinde1234/claude_meter) — this README follows the same "narrative handoff" style so a new chat (e.g., on another PC) can pick up without re-explaining everything.
**Purpose of this document:** Continuity reference. Claude Code conversation history is **local to the machine + folder path it was created in** (no cloud sync) — this file is the actual continuity mechanism across machines, not the chat transcript.

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

## 3. Toolchain (Decided)

Matches the reference project's philosophy: **no vendor-specific IDE.**

- **arduino-cli** — not the Arduino IDE GUI, not PlatformIO. VS Code is used purely as a text editor; compiling/flashing happens via CLI.
- Library stack (planned): `Wire.h` (built-in, I2C communication) + `Adafruit_SH110X` + `Adafruit_GFX` (+ `Adafruit_BusIO` auto-dependency).
- Toolchain setup/installation itself is **out of scope for the office PC session** — the user handles arduino-cli install/board config on their own machine(s). Don't attempt to install or verify it via shell commands.

## 4. Progress Log

### Concepts covered ✅

- [x] **What I2C is and why it's used** — 2-wire shared bus vs. dedicated wires per device; controller (ESP32) vs. peripheral (OLED) terminology
- [x] **SDA and SCL** — SCL as clock/timing ("drumbeat"), SDA as the actual data line, data read on clock edge
- [x] **Pull-up resistors** — open-drain wires (devices can only pull LOW or release), resistor pulls the line HIGH by default; OLED breakout likely has these built in
- [x] **Device addresses** — 7-bit address (e.g., OLED = `0x3C`) so multiple devices can share the same 2 wires; hex notation (`0x..`) intro; `#define` intro
- [x] **What the `Wire` library does** — abstracts away manual clock/data bit-banging; `#include <Wire.h>`, `setup()` as one-time init function, `Wire.begin()` defaults to GPIO21/22 on ESP32

### Decisions made along the way

- Initially considered raw ESP-IDF I2C driver (no Arduino library) — **superseded**. Final decision: match the reference repo exactly — arduino-cli + Wire.h + Adafruit libraries (see section 3).
- Confirmed OLED driver chip: **SH1106**, not SSD1306 (see section 2).

### Not yet covered (next steps, in order)

- [ ] `Wire.beginTransmission()` / `Wire.endTransmission()` — starting and ending an I2C "conversation" with a specific address
- [ ] Building an **I2C scanner** sketch (loops through addresses 1–127, reports which ones respond) — one of the user's explicit learning goals, doubles as a debugging tool
- [ ] Introducing the Adafruit display libraries (`Adafruit_GFX` + `Adafruit_SH110X`) and what each provides
- [ ] Writing the OLED init sequence (`display.begin()`, clearing the buffer, etc.) piece by piece
- [ ] Drawing text/shapes to the OLED buffer and pushing it with `display.display()`
- [ ] Assembling the complete working program from the pieces learned
- [ ] Debugging common I2C issues (no device found, wrong address, garbled display) using the scanner

## 5. How to Resume This Session Elsewhere

1. Clone/copy this project folder (including `Images/` and this `README.md`).
2. Start a new Claude Code session in that folder.
3. Point Claude at this README and say something like: *"Continue teaching me I2C from where README.md's progress log leaves off, following the same teaching rules in section 1."*
4. No need to re-explain hardware, wiring, toolchain decisions, or concepts already covered — they're all captured above.
