[← Back to Root Repository](../../README.md)

# ToneOS Firmware

**ESP32 Embedded Firmware for Tone Tactile Sound Controller**

ToneOS manages the physical peripherals of Tone: reads the analog 2-axis joystick with push-to-click detection, drives the 270-degree WS2812B NeoPixel diffuser arc, and provides a Bluetooth Low Energy (BLE 5.1) GATT server advertising as `Tone Equalizer`.

---

## Hardware Targets & Environments

The firmware is configured with [PlatformIO](https://platformio.org/) supporting two hardware targets in `platformio.ini`:

| Environment | Board ID | Architecture | Pinout Configuration |
| :--- | :--- | :--- | :--- |
| **`esp32dev`** *(default)* | `esp32dev` | Xtensa dual-core ESP32 | X: `GPIO34`, Y: `GPIO35`, SW: `GPIO25`, LED: `GPIO13` |
| **`esp32-c3-devkitm-1`** | `esp32-c3-devkitm-1` | RISC-V single-core ESP32-C3 | X: `GPIO3`, Y: `GPIO4`, SW: `GPIO5`, LED: `GPIO10` |

---

## Directory Structure

```text
libs/ToneOS/
├── platformio.ini              # PlatformIO environments and library dependencies
├── CMakeLists.txt              # IDE indexing (CLion / VS Code)
├── include/                    # Header declarations
│   ├── BluetoothController.h   # BLE GATT server and advertising
│   ├── JoyController.h         # Analog joystick normalization & deadzone
│   ├── PixelController.h       # NeoPixel ring illumination & animations
│   └── ToneController.h        # Mode state machine & event dispatcher
└── src/                        # Implementation files
    ├── main.cpp                # Setup, loop and hardware configuration
    ├── BluetoothController.cpp
    ├── JoyController.cpp
    ├── PixelController.cpp
    └── ToneController.cpp
```

---

## Build & Flash (PlatformIO)

### 1. Build Firmware

```bash
# Build default environment (ESP32 DevKit)
pio run

# Build specifically for ESP32-C3
pio run -e esp32-c3-devkitm-1
```

### 2. Upload to Device

```bash
# Upload to connected ESP32
pio run --target upload

# Upload for ESP32-C3
pio run -e esp32-c3-devkitm-1 --target upload
```

### 3. Serial Monitor

```bash
pio device monitor -b 115200
```
