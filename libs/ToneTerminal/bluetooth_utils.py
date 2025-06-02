import asyncio
import os
from bleak import BleakScanner, BLEDevice, BleakClient, BleakGATTServiceCollection, BleakGATTCharacteristic
from dotenv import load_dotenv
import json

from utils import log, set_volume_mac, get_volume_data_as_int

load_dotenv()

tone_device: BLEDevice = None  # Replace it with your device's MAC address
tone_client: BleakClient = None  # Replace it with your BLE client if needed
tone_device_name = os.getenv("DEVICE_NAME", "Tone Equalizer")
characteristic_uuid = os.getenv("CHARACTERISTIC_UUID", "your_characteristic_uuid_here")
service_uuid = os.getenv("SERVICE_UUID", "your_service_uuid_here")
operating_system = os.uname().sysname


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


# === Sync kullanımı için BLE sarmalayıcı ===
def is_ble_device_connected_sync(device_name: str) -> bool:
    return asyncio.run(is_ble_device_nearby(device_name))


def connect_to_ble_device() -> bool:
    global tone_device
    global tone_client
    global characteristic_uuid
    current_volume = get_volume_data_as_int(operating_system)
    current_vol_str = f"vol:{current_volume}"
    log(f"Current volume: {current_volume}")

    if tone_device is None:
        log("No BLE device found to connect.", "ERROR")
        return False
    try:
        async def connect():
            async with BleakClient(tone_device.address) as client:
                status = 'Connected' if client.is_connected else 'Not connected'
                log(f"{status} to {tone_device.name} ({tone_device.address})")
                await client.write_gatt_char(find_notify_uuid(client.services), current_vol_str.encode('utf-8'))
                await client.start_notify(find_notify_uuid(client.services), handle_notification)
                log("Listening for messages...")
                await asyncio.sleep(9999)
                return True

        return asyncio.run(connect())
    except Exception as e:
        log(f"Connection Error: {e}", "ERROR")
        return False


def handle_notification(sender: BleakGATTCharacteristic, data: bytearray):
    if not data:
        return
    try:
        str_data = data.decode('utf-8', errors='ignore')
        json_data = json.loads(str_data)
        log(f"From {sender.uuid}: {json_data}", "TONE")
        if operating_system == "Darwin":  # macOS
            print(json_data)
            # set_volume_mac(int(json_data.get("value", 0)))
        elif operating_system == "Windows":
            print(json_data)
            # set_volume_windows(int(json_data.get("value", 0)))

    except Exception as e:
        log(f"Error decoding data: {e}", "ERROR")
        return


def find_notify_uuid(client_services: BleakGATTServiceCollection):
    """
    Finds the first characteristic with notification support in the given service.
    """
    for service in client_services:
        for characteristic in service.characteristics:
            if 'notify' in characteristic.properties:
                return characteristic
    return None
