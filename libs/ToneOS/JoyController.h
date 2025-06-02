//
// Created by Ata Can Yaymacı on 26.04.2025.
//

#ifndef JOYCONTROLLER_H
#define JOYCONTROLLER_H

#include <Arduino.h>

/**
 * @brief JoystickController handles joystick input, including X and Y axis readings, button press detection
 * and calibration.
 */
class JoystickController {
private:
    uint8_t _vrxPin, _vryPin, _swPin;
    int originX, originY, deadZone;

    /**
     * @brief Normalizes the X axis value based on the origin.
     * @param x The raw X axis value from the joystick.
     * @return int The normalized X axis value.
     */
    int originizeX(int x);

    /**
     * @brief Normalizes the Y axis value based on the origin.
     * @param y The raw Y axis value from the joystick.
     * @return int The normalized Y axis value.
     */
    int originizeY(int y);

    /**
     * @brief Normalizes the value between 1 and -1.
     * @param value The raw value from the joystick.
     * @return float The normalized value.
     */
    float normalize(int value);

    /**
     * @brief Checks if the value is within the dead zone.
     * @param value The value to check.
     * @return true if the value is within the dead zone
     */
    bool atDeadZone(int value);

public:
    /**
     * @brief Initializes joystick controller with given pins.
     *
     * @param vrxPin Analog pin connected to the joystick X axis
     * @param vryPin Analog pin connected to the joystick Y axis
     * @param swPin Digital pin connected to the joystick button (switch)
     */
    JoystickController(uint8_t vrxPin, uint8_t vryPin, uint8_t swPin);

    /**
     * @brief Starts the joystick and calibrates the center position.
     */
    void begin();

    /**
     * @brief Calibrates the joystick by setting the current position as origin.
     */
    void calibrate();

    /**
     * @brief Reads and normalizes the X axis value.
     * @return int The normalized X axis value, or 0 if within dead zone.
     */
    int readX();

    /**
     * @brief Reads and normalizes the Y axis value.
     * @return int The normalized Y axis value, or 0 if within dead zone.
     */
    int readY();

    /**
     * @brief Calculates the angle (in degrees) based on X and Y axes.
     * @return int Angle in degrees between 0 and 360.
     */
    int readAngle(int offset=0);

    /**
     * @brief Checks if the joystick is centered (dead zone).
     * @return true if both X and Y are approximately zero
     */
    bool atOrigin();

    /**
     * @brief Checks if the joystick button is pressed.
     * @return true if button is pressed
     */
    bool isPressed();
};

#endif //JOYCONTROLLER_H
