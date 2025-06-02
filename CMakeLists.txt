cmake_minimum_required(VERSION 3.30)
project(toneOS)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/cores/esp32
        ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/variants/esp32
        ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/WiFi/src
        ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/BluetoothSerial/src
        ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/Preferences/src
        ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/ArduinoOTA/src
        ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/Update/src
)


add_executable(toneOS
        JoyController.cpp
        JoyController.h
        PixelController.cpp
        PixelController.h
        ToneController.cpp
        ToneController.h
        BluetoothController.h
        BluetoothController.cpp
        toneOS.ino)
