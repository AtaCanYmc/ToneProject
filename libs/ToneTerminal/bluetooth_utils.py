import asyncio
import os
import platform
import json
try:
    from dotenv import load_dotenv
    load_dotenv()
except ImportError:
    pass

try:
    from bleak import (
        BleakScanner,
        BLEDevice,
        BleakClient,
        BleakGATTServiceCollection,
        BleakGATTCharacteristic,
    )
except ImportError:
    BleakScanner = None
    BLEDevice = None
    BleakClient = None
    BleakGATTServiceCollection = None
    BleakGATTCharacteristic = None

from utils import (
    log,
    set_volume_mac,
    set_volume_windows,
    get_volume_data_as_int,
)

tone_device: BLEDevice = None
tone_client: BleakClient = None
tone_device_name = os.getenv("DEVICE_NAME", "Tone Equalizer")
characteristic_uuid = os.getenv("CHARACTERISTIC_UUID", "abcdefab-1234-1234-1234-abcdefabcdef")
service_uuid = os.getenv("SERVICE_UUID", "12345678-1234-1234-1234-1234567890ab")
operating_system = platform.system()


# === BLE (Bluetooth Low Energy) ===
async def is_ble_device_nearby(device_name: str) -> bool:
    try:
        devices = await BleakScanner.discover(timeout=5.0)
        for device in devices:
            if device.name and device_name.lower() in device.name.lower():
                return save_ble_device_nearby(device)
        return False
    except Exception as e:
        log(f"BLE Scan Error: {e}", "ERROR")
        return False


def save_ble_device_nearby(device: BLEDevice) -> bool:
    global tone_device
    if device.name and tone_device is None:
        tone_device = device
        log(f"Device found: {device.name} - {device.address}")
        return True
    return False


# === Sync ===
def is_ble_device_connected_sync(device_name: str) -> bool:
    return asyncio.run(is_ble_device_nearby(device_name))


def connect_to_ble_device() -> bool:
    global tone_device
    global tone_client
    current_volume = get_volume_data_as_int(operating_system)
    current_vol_str = f"vol:{current_volume}"
    log(f"Current host volume: {current_volume}%")

    if tone_device is None:
        log("No BLE device found to connect.", "ERROR")
        return False
    try:
        async def connect():
            async with BleakClient(tone_device.address) as client:
                status = "Connected" if client.is_connected else "Not connected"
                log(f"{status} to {tone_device.name} ({tone_device.address})")

                notify_char = find_notify_uuid(client.services)
                if not notify_char:
                    log("No notify characteristic found on device.", "ERROR")
                    return False

                # Send initial host volume to synchronize hardware display
                await client.write_gatt_char(notify_char, current_vol_str.encode("utf-8"))
                await client.start_notify(notify_char, handle_notification)
                log("Listening for tone notifications...")
                while client.is_connected:
                    await asyncio.sleep(1)
                return True

        return asyncio.run(connect())
    except Exception as e:
        log(f"Connection Error: {e}", "ERROR")
        return False


def handle_notification(sender: BleakGATTCharacteristic, data: bytearray):
    if not data:
        return
    try:
        str_data = data.decode("utf-8", errors="ignore").strip()
        json_data = json.loads(str_data)
        log(f"From {sender.uuid}: {json_data}", "TONE")

        raw_val = json_data.get("value")
        if raw_val is None:
            return

        volume_val = max(0, min(100, int(raw_val)))
        mode_name = json_data.get("mode", "Volume")

        if mode_name == "Volume":
            if operating_system == "Darwin":
                set_volume_mac(volume_val)
            elif operating_system in ("Windows", "nt"):
                set_volume_windows(volume_val)
        else:
            log(f"Equalizer profile active [{mode_name}]: {volume_val}%", "EQ")

    except (ValueError, TypeError, json.JSONDecodeError) as e:
        log(f"Error parsing BLE notification: {e}", "WARNING")
    except Exception as e:
        log(f"Unexpected error in notification handler: {e}", "ERROR")


def find_notify_uuid(client_services: BleakGATTServiceCollection):
    """
    Finds the first characteristic with notification support in the given services.
    """
    for service in client_services:
        for characteristic in service.characteristics:
            if "notify" in characteristic.properties:
                return characteristic
    return None
