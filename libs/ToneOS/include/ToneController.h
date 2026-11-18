//
// Created by Ata Can Yaymacı on 26.04.2025.
//

#ifndef TONECONTROLLER_H
#define TONECONTROLLER_H

#include <Arduino.h>
#include "PixelController.h"
#include "JoyController.h"
#include "BluetoothController.h"

/**
 * @brief Number of modes supported by the ToneController.
 * This constant defines how many different interaction modes can be configured.
 */
#define MODE_COUNT 3

/**
 * @brief Structure representing a mode, with a name, value range, current value, color and brightness.
 */
struct mode {
    String name;
    int minValue = 0;
    int maxValue = 100;
    int currentValue = 0;
    uint8_t color[3] = {255, 255, 255};
    uint8_t brightness = 150;
};

/**
 * @brief ToneController handles multiple interaction modes using a joystick and pixel LED ring.
 * It maps joystick input to angles and values, and provides vibration feedback and LED visualization.
 */
class ToneController {
private:
    int _xPin; ///< Analog pin for joystick X axis
    int _yPin; ///< Analog pin for joystick Y axis
    int _swPin; ///< Digital pin for joystick switch
    int _pixelPin; ///< Digital pin for LED ring
    int _pixelCount; ///< Number of pixels in the LED ring
    PixelController *pixel; ///< Pointer to PixelController instance
    JoystickController *joystick; ///< Pointer to JoystickController instance
    BluetoothController *bluetooth; ///< Pointer to BluetoothController instance
    mode modes[MODE_COUNT]; ///< Array of modes
    int currentModeIndex; ///< Index of the currently active mode

    /**
     * @brief Sets the current value of the active mode.
     * @param value The value to be set within the mode's range.
     */
    void setCurrentValue(int value);

    /**
     * @brief Maps a given joystick angle to the current mode's value range.
     * @param angle Angle in degrees (0–360).
     * @param maxAngle Maximum angle
     * @return Mapped value within mode's min and max.
     */
    int getMappedValue(int angle, int maxAngle=360);

    /**
     * @brief Calculates the number of LEDs to turn on
     * @param value The value to be set within the mode's range.
     */
    int getMappedPixelIndex(int value);

public:
    /**
         * @brief Construct a new ToneController object with all necessary pins.
         * @param xPin Analog pin for joystick X axis.
         * @param yPin Analog pin for joystick Y axis.
         * @param swPin Digital pin for joystick switch.
         * @param pixelPin Digital pin connected to the LED ring.
         * @param pixelCount Number of pixels in the LED ring.
         */
    ToneController(int xPin, int yPin, int swPin, int pixelPin, int pixelCount);

    /**
     * @brief Initializes joystick, pixel controller and other hardware.
     */
    void begin();

    /**
     * @brief Updates the system – reads joystick, updates mode value, vibration and LEDs.
     * Should be called regularly in loop().
     */
    void update();

    /**
     * @brief Sets the configuration for a specific mode.
     * @param index Index of the mode (0 to MODE_COUNT-1).
     * @param name Mode display name.
     * @param minValue Minimum value of the mode.
     * @param maxValue Maximum value of the mode.
     * @param r Red color component (0–255).
     * @param g Green color component (0–255).
     * @param b Blue color component (0–255).
     * @param brightness LED brightness (0–255).
     */
    void setMode(int index, String name, int minValue, int maxValue, uint8_t r, uint8_t g, uint8_t b,
                 uint8_t brightness);

    /**
     * @brief Activates the specified mode by index.
     * @param index Mode index to activate.
     */
    void setCurrentMode(int index);

    /**
     * @brief Switches to the next mode in the list (looping).
     */
    void nextMode();

    /**
     * @brief Returns the current value of the active mode.
     * @return int Current value.
     */
    int getCurrentValue() const;

    /**
     * @brief Gets the name of the currently active mode.
     * @return String Mode name.
     */
    String getCurrentModeName();

    /**
     * @brief Sends the current mode data over Bluetooth.
     * This includes mode name, current value, and color.
     */
    void sendDataChange();

    static float mapf(float x, float in_min, float in_max, float out_min, float out_max);
};

#endif //TONECONTROLLER_H
