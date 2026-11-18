# Tone User Manual & Operation Guide

Welcome to Tone, your tactile desktop audio controller. This guide covers setup, pairing, operation, maintenance, and troubleshooting.

---

## 1. Unboxing & Package Contents

Carefully unpack the shipping carton and verify all contents:
- 1x **Tone Audio Controller** (Model TNE-01)
- 1x **USB-C Charging & Data Cable**
- 1x **User Reference Card**

---

## 2. Initial Setup & Pairing

1. **Powering On**:
   Press and hold the power button located on the base for 3 seconds until the optical LED ring illuminates.
2. **Bluetooth Discovery**:
   On your computer (macOS or Windows), ensure Bluetooth is enabled.
3. **Run Companion Terminal**:
   Launch `ToneTerminal` to establish automated pairing:
   ```bash
   cd libs/ToneTerminal
   python3 main.py
   ```
4. **Connection Confirmation**:
   The terminal will log `Connected to Tone Equalizer`. The LED ring will reflect your host system's current volume level.

---

## 3. Daily Operation

- **Adjust Level**: Rotate the top tactile disk clockwise to increase the level ($0 - 100$) and counter-clockwise to decrease it.
- **Switch Parameter**: Tap down on the central sphere to cycle through parameters:
  - **White LED**: Volume
  - **Purple LED**: Bass
  - **Cyan LED**: Treble
- **Power Off**: Hold the base power button for 3 seconds until the LEDs fade out.

---

## 4. Maintenance & Cleaning

- Always power off Tone and disconnect any charging cables before cleaning.
- Wipe the exterior aluminum and frosted diffuser gently using a dry, clean microfiber cloth.
- **Do not** use chemical solvents, rubbing alcohol, bleach, or abrasive pads.
- Keep the device away from liquids, high-humidity environments, and direct radiant heat.

---

## 5. Troubleshooting Guide

| Issue | Potential Cause | Remediation |
| :--- | :--- | :--- |
| **Device not discovered over Bluetooth** | Device is powered off or paired with another host | Hold power button for 3 seconds. Disconnect prior host. Verify Bluetooth permissions in terminal. |
| **LED ring remains dark** | Battery discharged or sleep timeout active | Connect USB-C charger (5V / 2A). Tap the center stick to wake from low-power state. |
| **Volume does not adjust on macOS** | Terminal lacks AppleScript execution permission | Grant Terminal or IDE permission in macOS **System Settings** $\rightarrow$ **Privacy & Security** $\rightarrow$ **Automation**. |
| **Windows volume errors** | Audio device COM endpoint disconnected | Ensure playback device is active and speakers/headphones are detected as default endpoint. |

---

## 6. Project Credits & Design Attribution

- **Product Designer**: **Sezin Sözden** ([sezinsozden@gmail.com](mailto:sezinsozden@gmail.com) · [Portfolio](https://sezinsozden.my.canva.site/portfolio))
- **Firmware & Software**: **Ata Can Yaymacı** ([@AtaCanYmc](https://github.com/AtaCanYmc))
