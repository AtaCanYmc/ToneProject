import json
import subprocess
import platform
from datetime import datetime


def log(message, level="INFO"):
    """
    Logs a message with a specified log level.

    Args:
        message (str): The message to log.
        level (str): The log level (e.g., "INFO", "WARNING", "ERROR"). Defaults to "INFO".
    """
    print(f"[{get_timestamp()}] [{level}]: {message}")


def get_timestamp():
    """
    Returns the current timestamp in a human-readable format.

    Returns:
        str: The current timestamp.
    """
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def set_volume_mac(percent: int):
    """
    Sets master audio volume on macOS (0-100).
    """
    try:
        volume = max(0, min(100, int(percent)))
        apple_script = f"set volume output volume {volume}"
        subprocess.run(["osascript", "-e", apple_script], check=True, capture_output=True)
    except Exception as e:
        log(f"Failed to set macOS volume: {e}", "ERROR")


def get_volume_mac() -> int:
    """
    Retrieves current master audio volume on macOS.
    """
    try:
        result = subprocess.run(
            ["osascript", "-e", "output volume of (get volume settings)"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=3
        )
        if result.returncode == 0:
            val_str = result.stdout.strip()
            if val_str.isdigit():
                return int(val_str)
    except Exception as e:
        log(f"Failed to query macOS volume: {e}", "WARNING")
    return 50


def set_volume_windows(percent: int):
    """
    Sets master audio volume on Windows (0-100).
    """
    try:
        from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
        from ctypes import cast, POINTER
        from comtypes import CLSCTX_ALL

        volume = max(0, min(100, int(percent)))
        devices = AudioUtilities.GetSpeakers()
        if not devices:
            log("No active audio playback devices found.", "WARNING")
            return
        interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
        volume_control = cast(interface, POINTER(IAudioEndpointVolume))
        volume_control.SetMasterVolumeLevelScalar(volume / 100.0, None)
    except Exception as e:
        log(f"Failed to set Windows volume: {e}", "ERROR")


def get_volume_windows() -> int:
    """
    Retrieves current master audio volume on Windows.
    """
    try:
        from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
        from ctypes import cast, POINTER
        from comtypes import CLSCTX_ALL

        devices = AudioUtilities.GetSpeakers()
        if not devices:
            return 50
        interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
        volume = cast(interface, POINTER(IAudioEndpointVolume))
        current = volume.GetMasterVolumeLevelScalar()
        return max(0, min(100, int(round(current * 100))))
    except Exception as e:
        log(f"Failed to query Windows volume: {e}", "WARNING")
        return 50


def get_volume_data_as_int(os_name: str) -> int:
    """
    Returns the current volume as integer for the detected host OS.
    """
    if os_name == "Darwin":
        return get_volume_mac()
    elif os_name in ("Windows", "nt"):
        return get_volume_windows()
    return 50
