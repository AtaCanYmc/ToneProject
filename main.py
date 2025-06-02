from bluetooth_utils import *
from utils import log

if __name__ == '__main__':
    isDeviceNearby = is_ble_device_connected_sync('Tone Equalizer')
    if not isDeviceNearby:
        log("Waiting for device to be nearby...", "BLE")

    while not isDeviceNearby:
        isDeviceNearby = is_ble_device_connected_sync('Tone Equalizer')

    log("Device is nearby, proceeding with the application.", "BLE")
    connect_to_ble_device()
