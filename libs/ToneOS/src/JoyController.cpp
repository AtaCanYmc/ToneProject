#include "HardwareSerial.h"
//
// Created by Ata Can Yaymacı on 26.04.2025.
//

#include "JoyController.h"

JoystickController::JoystickController(uint8_t vrxPin, uint8_t vryPin, uint8_t swPin)
    : _vrxPin(vrxPin), _vryPin(vryPin), _swPin(swPin) {
    pinMode(_vrxPin, INPUT);
    pinMode(_vryPin, INPUT);
    pinMode(_swPin, INPUT_PULLUP); // Use internal pull-up resistor for switch
}

void JoystickController::begin() {
    delay(500); // Allow time for the joystick to stabilize)
    this->deadZone = 450;
    this->calibrate(); // Calibrate the joystick
}

void JoystickController::calibrate() {
    // Calibrate the joystick by setting the origin to the current position
    originX = analogRead(_vrxPin);
    originY = analogRead(_vryPin);
}

int JoystickController::originizeX(int x) {
    // reassing x within originX
    return x - originX;
}

int JoystickController::originizeY(int y) {
    // reassing y within originY
    return y - originY;
}

float JoystickController::normalize(int value) {
    return (abs(value) - 512) / 512.0;
}

bool JoystickController::atDeadZone(int value) {
    return abs(value) < deadZone;
}

int JoystickController::readX() {
    int read =  analogRead(_vrxPin);
    int normalized =  originizeX(read);
    if (atDeadZone(normalized)) return 0;
    return normalized;
}

int JoystickController::readY() {
    int read =  analogRead(_vryPin);
    int normalized =  originizeY(read);
    if (atDeadZone(normalized)) return 0;
    return normalized;
}

int JoystickController::readAngle(int offset) {
    float x = readX();
    float y = readY();
    float radian = atan2(y, x); // atan2 returns angle in radians
    float angle = radian * (180.0 / PI); // Convert radians to degrees
    angle += offset;
    while (angle < 0) angle += 360;
    return (int) round(angle) % 360;
}

bool JoystickController::atOrigin() {
    int x = readX();
    int y = readY();
    return (x == 0 && y == 0); // Check if joystick is at origin
}

bool JoystickController::isPressed() {
    return digitalRead(_swPin) == LOW; // Button pressed when pin is LOW
}
