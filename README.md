# Robatic TVTO

Competition robotics software for two robots, developed for Iran's national and international WorldSkills Mobile Robotics competitions. Each robot has its own control program that runs on a Windows PC and communicates with hardware over serial ports.

---

## Robots

### Robot 1 — Plates (`main_plates/`) 

The original robot, built for **Iran's national work skills competition**. It is a Mecanum-wheel mobile platform that navigates autonomously and manipulates flat objects (plates) using color detection and distance sensors.

[![National Competition Demo](https://img.youtube.com/vi/ygSOpZgD2VM/hqdefault.jpg)](https://www.youtube.com/watch?v=ygSOpZgD2VM)

**Hardware:**
- Mecanum-wheel base driven by 4 DC motors (controllers on COM8 and COM7)
- USB webcam for HSV color detection
- Arduino Mega (COM6) for laser distance sensors, Sharp IR sensors, and LEDs

---

### Robot 2— Arm (`main_arm/`) 

An evolution of Robot 1, modified and extended for the **international WorldSkills Mobile Robotics competition**. The Mecanum base was kept and a full robotic arm was added on top, along with barcode scanning and more complex pick-and-place scenarios.

[![International Competition Demo](https://img.youtube.com/vi/7hcjavu2HdQ/hqdefault.jpg)](https://www.youtube.com/watch?v=7hcjavu2HdQ&t=166s)

**Hardware:**
- 7 Dynamixel servo motors (MX-64, MX-106, MX-28, AX-series) for arm joints and gripper
- Mecanum-wheel base driven by 4 DC motors (controllers on COM10 and COM3)
- USB webcam for HSV color detection and barcode scanning
- Arduino Mega (COM4) for laser distance sensors, Sharp IR sensors, and LEDs

---

## Hardware Architecture

```
Windows PC
  ├── Dynamixel motors      ← serial COM port (Dynamixel protocol)
  ├── Arduino Mega          ← serial 38400 baud
  │     ├── DC motor controllers  (I2C, addr 0x58)
  │     ├── Sharp IR sensors      (analog)
  │     ├── Laser distance sensors
  │     └── LEDs                  (GPIO 21–28)
  └── USB webcam            ← OpenCV VideoCapture
```

---

## Software Architecture

Both programs share the same module structure:

| Module | Description |
|---|---|
| `ReadWrite` | Win32 serial port wrapper (all hardware communication goes through this) |
| `dynamixel` | Dynamixel servo protocol — packet TX/RX, position/speed control |
| `motors` | Mecanum-wheel kinematics and encoder-based distance control |
| `arduino` | Sensor reading and LED control via Arduino over serial |
| `opencv` | HSV color detection, barcode/QR reading, audio feedback |
| `Place` | Represents a target placement zone (position, expected color, completion state) |
| `main` / `Source` | Top-level scenario logic — object detection, pick-and-place sequencing |

---

## Dependencies

- **Visual Studio** (Windows) — the project is Windows-only
- **OpenCV 2.x** — computer vision (color detection uses the legacy C API)
- **Dynamsoft Barcode Reader** (`DBRx64.lib` + `If_DBRP.h`) — barcode/QR scanning (arm robot only)
- **winmm.lib** — WAV audio playback (included with Windows SDK)

Runtime DLLs (`msvcp100.dll`, `msvcr100.dll`, `msvcr120.dll`) are in `resources/libs/`.

---

## Arduino Firmware

Upload the appropriate sketch from `Arduino/` to an **Arduino Mega** using Arduino IDE:

- `laser.ino` — full sensor suite (3 serial ports, 6 relays, 8 Sharp IR sensors, laser distance sensors, checksum protocol)
- `blink_led.ino` — LED control + I2C motor driver interface

---

## Repository Structure

```
main_arm/           # Arm robot control program (international competition)
main_plates/        # Plates robot control program (national competition)
Arduino/            # Arduino firmware
Documents/          # Competition specifications and criteria
Materials/          # Hardware references (Mecanum wheel)
```

Both `main_arm/` and `main_plates/` contain their own copy of all source files and a `resources/` folder with audio, pattern, and barcode images needed at runtime.

---

## Branch Structure

| Branch | Description |
|---|---|
| `main` | Stable base |
| `v2/first_day` | Competition day 1 |
| `v2/first_day_accurate` | Day 1 with accuracy improvements |
| `v2/second_day` | Competition day 2 |
| `v2/second_day_stop_image` | Day 2 with stop-image variant |

---

> **Note:** `Documents/national2016Presentation.pptx` is stored via Git LFS. Run `git lfs install` before cloning.
