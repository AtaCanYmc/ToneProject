[← Back to Root Repository](../../README.md)

# ToneTerminal

**Cross-Platform Python Desktop Companion for Tone Audio Controller**

ToneTerminal runs as a lightweight desktop service on macOS and Windows. It continuously scans for the Tone hardware peripheral (`Tone Equalizer`) over Bluetooth Low Energy (BLE 5.1), performs initial volume synchronization, and listens for real-time mode and volume notifications to adjust the host system audio mixer.

---

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Running ToneTerminal](#running-toneterminal)
- [Running Tests](#running-tests)
- [Audio Engine Mechanics](#audio-engine-mechanics)

---

## Features

- **Automated BLE Reconnection**: Scans for `Tone Equalizer` and establishes connection with zero manual pairing prompts.
- **Bi-directional Volume Handshake**: Immediately queries the host OS volume upon connection and writes `vol:<level>` to the peripheral to synchronize the optical NeoPixel arc.
- **Equalizer & Mode Segregation**: Distinguishes between `Volume` mode and equalizer profiles (`Bass`, `Treble`). EQ updates are logged and tracked without overriding master output gain.
- **Defensive Bounds Clamping**: All received audio updates are strictly constrained to $[0, 100]$ to prevent unexpected audio spikes or script injection.
- **Platform-Aware Dependencies**: `requirements.txt` specifies Windows-only libraries (`pycaw`, `comtypes`) with PEP 508 platform markers (`sys_platform == 'win32'`), preventing installation failures on macOS and Linux.

---

## Requirements

- **Python**: 3.10 or newer
- **Operating System**: macOS 12+ (Darwin) or Windows 10/11 (NT)
- **Hardware**: Bluetooth 4.2+ Low Energy adapter

---

## Installation

```bash
# 1. Navigate to ToneTerminal
cd libs/ToneTerminal

# 2. Create and activate a virtual environment
python3 -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# 3. Install dependencies
pip install -r requirements.txt
```

---

## Configuration

ToneTerminal looks for environment variables in an optional `.env` file:

```bash
cp .env.example .env
```

| Variable | Default Value | Description |
| :--- | :--- | :--- |
| `DEVICE_NAME` | `Tone Equalizer` | Advertised BLE local device name to discover. |
| `SERVICE_UUID` | `12345678-1234-1234-1234-1234567890ab` | GATT Service UUID configured in ToneOS. |
| `CHARACTERISTIC_UUID` | `abcdefab-1234-1234-1234-abcdefabcdef` | GATT Characteristic UUID for notifications. |

---

## Running ToneTerminal

```bash
python3 main.py
```

### Expected Output
```text
[2026-11-18 15:20:00] [INFO]: Starting ToneTerminal (target device: 'Tone Equalizer')...
[2026-11-18 15:20:00] [BLE]: Scanning for 'Tone Equalizer' nearby...
[2026-11-18 15:20:02] [BLE]: Device found: Tone Equalizer - XX:XX:XX:XX:XX:XX
[2026-11-18 15:20:02] [INFO]: Current host volume: 65%
[2026-11-18 15:20:02] [INFO]: Connected to Tone Equalizer (XX:XX:XX:XX:XX:XX)
[2026-11-18 15:20:02] [INFO]: Listening for tone notifications...
```

To stop ToneTerminal, press `Ctrl+C`. The process handles `SIGINT` gracefully and exits cleanly.

---

## Running Tests

ToneTerminal uses Python's standard library `unittest` framework. Tests can be run without any third-party dependencies:

```bash
python3 -m unittest discover tests
```

---

## Audio Engine Mechanics

### macOS (Darwin)
Interacts with the system CoreAudio daemon via sanitized AppleScript calls:
- Query volume: `osascript -e "output volume of (get volume settings)"`
- Set volume: `osascript -e "set volume output volume <0-100>"`

### Windows (NT)
Interacts with the Windows Core Audio APIs via `pycaw` (`IAudioEndpointVolume`):
- Scalar normalization: `volume_control.SetMasterVolumeLevelScalar(percent / 100.0, None)`
