import json
import subprocess
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
    volume = max(0, min(100, percent))  # 0–100 arasında sınırlama
    apple_script = f"set volume output volume {volume}"
    subprocess.run(["osascript", "-e", apple_script])


def get_volume_mac():
    result = subprocess.run(
        ["osascript", "-e", "output volume of (get volume settings)"],
        stdout=subprocess.PIPE
    )
    return int(result.stdout.decode().strip())


def set_volume_windows(percent: int):
    from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
    from ctypes import cast, POINTER
    from comtypes import CLSCTX_ALL

    volume = max(0, min(100, percent))  # sınırla
    devices = AudioUtilities.GetSpeakers()
    interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
    volume_control = cast(interface, POINTER(IAudioEndpointVolume))

    # Volume 0.0 to 1.0 arasında float olarak ayarlanır
    volume_control.SetMasterVolumeLevelScalar(volume / 100.0, None)


def get_volume_windows():
    from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
    from ctypes import cast, POINTER
    from comtypes import CLSCTX_ALL

    devices = AudioUtilities.GetSpeakers()
    interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
    volume = cast(interface, POINTER(IAudioEndpointVolume))
    current = volume.GetMasterVolumeLevelScalar()
    return int(current * 100)


def get_volume_data_as_int(os_name: str) -> int:
    if os_name == "Darwin":
        current_volume = get_volume_mac()
    elif os_name == "Windows":
        current_volume = get_volume_windows()
    else:
        current_volume = 50  # fallback
    return current_volume
