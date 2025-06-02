//
// Created by Ata Can Yaymacı on 4.05.2025.
//

#ifndef BluetoothController_h
#define BluetoothController_h

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcdefab-1234-1234-1234-abcdefabcdef"

/**
 * @brief Key-Value Pair structure for logging data.
 * This structure is used to represent a key-value pair for logging purposes.
 */
struct KVP {
    String key{};
    String value{};
};

/**
 * @brief BluetoothController handles Bluetooth Low Energy (BLE) communication.
 * It initializes the BLE server, manages connections, and sends/receives data.
 */
class BluetoothController {
public:
    /**
     * @brief Constructor for BluetoothController.
     * @param deviceName Name of the Bluetooth device.
     */
    explicit BluetoothController(const String &deviceName);  // Constructor

    /**
     * @brief Initializes the Bluetooth device and starts advertising.
     * This method sets up the BLE service and characteristic, and starts advertising.
     */
    void begin();

    /**
     * @brief Updates the Bluetooth connection state.
     * This method checks if the device is connected and starts advertising if it is.
     */
    void update();

    void log(const String &message);  // Logs messages in JSON format
    void log(const String &key, const String &value);  // Logs key-value pairs in JSON format
    void log(const KVP *kvp, int numKVP);  // Logs multiple key-value pairs

    void connect();  // Connects to the BLE device
    void disconnect();  // Disconnects from the BLE device

    /**
     * @brief Sends data over BLE.
     * @param kvp Array of key-value pairs to send.
     * @param numKVP Number of key-value pairs.
     */
    void sendData(const KVP *kvp, int numKVP);

    /**
     * @brief Receives data from BLE.
     * This method checks if there is any data available and prints it to the serial monitor.
     */
    String receiveData();  // Receives data from BLE

    /**
     * @brief Checks if the device is connected.
     * @return true if connected, false otherwise.
     */
    bool isConnected() const;  // Checks if the device is connected

private:
    int _baudRate{};
    String _deviceName{};
    bool _isConnected;
    bool _hasClient;
    BLEServer* _bleServer{};  // BLE server object
    BLEAdvertising* _bleAdvertising{};  // Advertising object for discoverability
    BLEService* _bleService{};  // BLE service
    BLECharacteristic* _bleCharacteristic{};  // BLE characteristic for communication

    class MyServerCallbacks : public BLEServerCallbacks {  // Callback class for BLE connection events
    public:
        explicit MyServerCallbacks(BluetoothController* controller) : _controller(controller) {}
        void onConnect(BLEServer* pServer) override;
        void onDisconnect(BLEServer* pServer) override;

    private:
        BluetoothController* _controller;
    };
};

#endif  // BluetoothController_h
