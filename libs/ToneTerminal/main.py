import sys
import time
from bluetooth_utils import is_ble_device_connected_sync, connect_to_ble_device, tone_device_name
from utils import log


def main():
    log(f"Starting ToneTerminal (target device: '{tone_device_name}')...", "INFO")
    try:
        is_nearby = is_ble_device_connected_sync(tone_device_name)
        if not is_nearby:
            log(f"Scanning for '{tone_device_name}' nearby...", "BLE")

        while not is_nearby:
            time.sleep(1)
            is_nearby = is_ble_device_connected_sync(tone_device_name)

        log(f"Device '{tone_device_name}' found nearby. Establishing connection...", "BLE")
        connect_to_ble_device()
    except KeyboardInterrupt:
        log("ToneTerminal stopped by user. Exiting cleanly.", "INFO")
        sys.exit(0)
    except Exception as e:
        log(f"Fatal error in ToneTerminal runtime: {e}", "ERROR")
        sys.exit(1)


if __name__ == '__main__':
    main()
