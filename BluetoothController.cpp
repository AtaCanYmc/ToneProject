//
// Created by Ata Can Yaymacı on 4.05.2025.
//

#include "BluetoothController.h"

/**
 * @brief BluetoothController handles Bluetooth Low Energy (BLE) communication.
 * It initializes the BLE server, manages connections, and sends/receives data.
 * @param deviceName Name of the Bluetooth device.
 */
BluetoothController::BluetoothController(const String &deviceName)
    : _deviceName(deviceName), _isConnected(false), _hasClient(false) {
    BLEDevice::init(_deviceName);
    _bleServer = BLEDevice::createServer();
    _bleAdvertising = _bleServer->getAdvertising();
    _bleService = _bleService = _bleServer->createService(SERVICE_UUID);
    _bleCharacteristic = _bleService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_WRITE
    );
}

/**
 * @brief Initializes the Bluetooth device and starts advertising.
 * This method sets up the BLE service and characteristic and starts advertising.
 */
void BluetoothController::begin() {
    BLEDevice::init(_deviceName.c_str());

    _bleServer = BLEDevice::createServer();
    _bleServer->setCallbacks(new MyServerCallbacks(this));

    _bleService = _bleServer->createService(SERVICE_UUID);

    _bleCharacteristic = _bleService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_WRITE
    );

    _bleCharacteristic->addDescriptor(new BLE2902());
    _bleCharacteristic->setValue("Ready");
    _bleService->start();

    _bleAdvertising = BLEDevice::getAdvertising();
    _bleAdvertising->addServiceUUID(SERVICE_UUID);
    _bleAdvertising->start();
}

/**
 * @brief Updates the Bluetooth connection state.
 * This method checks if the device is connected and starts advertising if it is.
 */
void BluetoothController::update() {
    if (_isConnected) {
        BLEDevice::startAdvertising();
    }
}

/**
 * @brief Logs a message in JSON format.
 * @param message The message to log.
 */
void BluetoothController::log(const String &message) {
    Serial.println(message);
}

/**
 * @brief Logs a key-value pair in JSON format.
 * @param key The key of the data.
 * @param value The value of the data.
 */
void BluetoothController::log(const String &key, const String &value) {
    String message = "{\"" + key + "\": \"" + value + "\"}";
    Serial.println(message);
}

/**
 * @brief Logs multiple key-value pairs in JSON format.
 * @param kvp Array of key-value pairs.
 * @param numKVP Number of key-value pairs.
 */
void BluetoothController::log(const KVP *kvp, int numKVP) {
    String message = "{";
    for (int i = 0; i < numKVP; i++) {
        message += "\"" + String(kvp[i].key) + "\": \"" + String(kvp[i].value) + "\"";
        if (i < numKVP - 1) {
            message += ", ";
        }
    }
    message += "}";
    Serial.println(message);
}

/**
 * @brief Connects to the BLE device.
 * This method starts advertising the BLE service and makes the device discoverable.
 */
void BluetoothController::connect() {
    if (!_isConnected) {
        _bleServer->startAdvertising();
        Serial.println("[BLE] - Bluetooth device is now discoverable.");
    }
}

/**
 * @brief Disconnects from the BLE device.
 * This method stops advertising the BLE service and makes the device undiscoverable.
 */
void BluetoothController::disconnect() {
    if (_isConnected) {
        _bleAdvertising->stop();
        Serial.println("[BLE] - Bluetooth device is no longer discoverable.");
    }
}

/**
 * @brief Sends data over BLE.
 * @param kvp Array of key-value pairs to send.
 * @param numKVP Number of key-value pairs.
 */
void BluetoothController::sendData(const KVP *kvp, int numKVP) {
    String message = "{";
    for (int i = 0; i < numKVP; i++) {
        message += "\"" + String(kvp[i].key) + "\": \"" + String(kvp[i].value) + "\"";
        if (i < numKVP - 1) {
            message += ", ";
        }
    }
    message += "}";
    _bleCharacteristic->setValue(message.c_str());
    _bleCharacteristic->notify();
}

/**
 * @brief Receives data from BLE.
 * This method checks if there is any data available and prints it to the serial monitor.
 * @return The received data as a String.
 */
String BluetoothController::receiveData() {
    if (_bleCharacteristic->getValue().length() > 0) {
        String data = _bleCharacteristic->getValue().c_str();
        Serial.println("[BLE] - Received data: " + data);
        return data;
    }

    return "";
}

/**
 * @brief Checks if the device is connected.
 * @return true if connected, false otherwise.
 */
bool BluetoothController::isConnected() const {
    return _isConnected;
}

/**
 * @brief Callback for BLE server connection events.
 * This class handles the connection and disconnection events of the BLE server.
 */
void BluetoothController::MyServerCallbacks::onConnect(BLEServer *pServer) {
    _controller->_isConnected = true;
    _controller->_hasClient = true;
    Serial.println("Client connected.");
}

/**
 * @brief Callback for BLE server disconnection events.
 * This class handles the disconnection event of the BLE server.
 */
void BluetoothController::MyServerCallbacks::onDisconnect(BLEServer *pServer) {
    _controller->_isConnected = false;
    _controller->_hasClient = false;
    Serial.println("[BLE] - Client disconnected.");
}

// End of BluetoothController.cpp
