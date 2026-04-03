# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Competition robotics codebase (RoboCup/WorldSkills style) for two robots:
- **main_arm/** — Robotic arm with Dynamixel servo motors + vision (entry point: `main.cpp`)
- **main_plates/** — Mobile robot platform, similar architecture (entry point: `Source.cpp`)

The code is **Windows-only** C++ (uses `<windows.h>`, Win32 serial API, `winmm.lib`, Dynamsoft Barcode Reader). No build system is checked in — Visual Studio is the intended IDE.

## Hardware Architecture

```
PC (C++ software)
  ├─→ Dynamixel MX/AX series motors   (serial COM port, Dynamixel protocol)
  ├─→ Arduino Mega (serial 38400 baud)
  │    ├─→ DC motor controllers       (I2C, address 0x58)
  │    ├─→ Sharp IR distance sensors  (analog)
  │    ├─→ IR reflection sensors      (analog)
  │    └─→ LED indicators             (GPIO pins 21–28)
  └─→ USB webcam                      (OpenCV VideoCapture)
```

## Module Responsibilities

| File | Role |
|------|------|
| `ReadWrite.cpp/h` | Win32 COM port wrapper (CreateFile/ReadFile/WriteFile) — used by all serial devices |
| `dynamixel.cpp/h` | Dynamixel servo protocol: raw packet TX/RX, sync write, high-level read/write |
| `motors.cpp/h` | Mecanum drive: 4-wheel kinematics (x, y, ω), encoder feedback via 2 COM ports |
| `arduino.cpp/h` | Serial framing to/from Arduino: laser distance sensors, Sharp IR, LEDs |
| `opencv.cpp/h` | HSV color detection (10+ colors), barcode reading (Dynamsoft SDK), audio via `mciSendString` |
| `Place.cpp/h` | Data structure for a target placement (position, color, barcode code, completion status) |
| `main.cpp` / `Source.cpp` | Top-level control loop: scenario sequencing, threading, object pick-and-place logic |

`main_arm/` and `main_plates/` are independent copies — they share the same module design but differ in COM port numbers, motor IDs, and scenario logic.

## Key Dependencies (not in repo)

- **OpenCV 2.x** — `IplImage`/legacy C API (`cvFindContours`, `cvMoments`) is used heavily
- **Dynamsoft Barcode Reader** (`DBRx64.lib` + `If_DBRP.h`) — barcode/QR scanning in `main_arm` only
- **winmm.lib** — WAV audio playback for color announcements
- **Windows MSVC runtime** — `msvcp100.dll`, `msvcr100.dll`, `msvcr120.dll` (in `resources/libs/`)

## Serial Port Assignments

| Port | Used by |
|------|---------|
| COM10, COM3 | Mecanum drive motors (main_arm) |
| COM4, 38400 | Arduino Mega (main_arm) |
| COM8, COM7 | Mecanum drive motors (main_plates) |
| COM6, 38400 | Arduino Mega (main_plates) |
| Dynamixel port | Set via `dxl_initialize(devIndex, baudnum)` in `dynamixel.cpp` |

## Runtime Resources

Must be present in the working directory at runtime:
- `resources/audio/` — WAV files referenced by name in `opencv.cpp` (e.g. `red.wav`, `blue.wav`)
- `resources/patterns/` — JPG images for pattern matching
- `resources/barcodes/` — test barcode images
- `resources/libs/` — MSVC runtime DLLs

## Branch Structure

- `main` — stable base
- `v2/first_day`, `v2/first_day_accurate` — competition day-1 variants
- `v2/second_day`, `v2/second_day_stop_image` — competition day-2 variants

## Arduino Firmware

Upload `Arduino/laser.ino` or `Arduino/blink_led.ino` via Arduino IDE to an **Arduino Mega**. `laser.ino` uses 3 serial ports (`Serial`, `Serial1`, `Serial2`) with a custom 4-byte checksum packet protocol (`FF FF data checksum`).

## Large Files

`Documents/national2016Presentation.pptx` (345 MB) is tracked via **Git LFS** — run `git lfs install` before cloning.
