//
// Created by Ata Can Yaymacı on 26.04.2025.
//

#include "ToneController.h"

ToneController::ToneController(int xPin, int yPin, int swPin, int pixelPin, int pixelCount) {
    _xPin = xPin;
    _yPin = yPin;
    _swPin = swPin;
    _pixelPin = pixelPin;
    _pixelCount = pixelCount;
}

void ToneController::begin() {
    pinMode(_xPin, INPUT);
    pinMode(_yPin, INPUT);
    pinMode(_swPin, INPUT_PULLUP);
    pinMode(_pixelPin, OUTPUT);

    // Initialize pixel controller
    this->pixel = new PixelController(_pixelPin, _pixelCount, 0, true);
    pixel->begin();

    // Initialize joystick
    this->joystick = new JoystickController(_xPin, _yPin, _swPin);
    this->joystick->begin();

    // Initialize Bluetooth controller
    this->bluetooth = new BluetoothController("Tone Equalizer");
    this->bluetooth->begin();

    // Set initial mode
    this->currentModeIndex = 0;
}

void ToneController::update() {
    if (joystick->isPressed()) {
        this->nextMode();
        while (joystick->isPressed()) {
        } // waiting
        return;
    } else if (joystick->atOrigin()) {
        return;
    }

    int angle = joystick->readAngle(90);
    int mappedValue = this->getMappedValue(angle, 330);
    if (mappedValue == -1 || mappedValue == modes[currentModeIndex].currentValue) {
        return;
    } 

    this->setCurrentValue(mappedValue);
    this->sendDataChange();
}

void ToneController::setMode(int index, String name, int minValue, int maxValue, uint8_t r, uint8_t g, uint8_t b,
                             uint8_t brightness) {
    if (index >= 0 && index < MODE_COUNT) {
        modes[index].name = name;
        modes[index].minValue = minValue;
        modes[index].maxValue = maxValue;
        modes[index].currentValue = minValue; // Initialize current value to min
        modes[index].color[0] = r;
        modes[index].color[1] = g;
        modes[index].color[2] = b;
        modes[index].brightness = brightness;
    }
}

void ToneController::nextMode() {
    this->setCurrentMode(this->currentModeIndex + 1);
}

void ToneController::setCurrentMode(int index) {
    index = index % MODE_COUNT;
    pixel->blink(modes[index].color[0],
                 modes[index].color[1],
                 modes[index].color[2],
                 1000);
    this->currentModeIndex = index;
    int current = modes[index].currentValue;
    int led_index = this->getMappedPixelIndex(current);
    pixel->setFirstnPixelColor(
        led_index,
        modes[index].color[0],
        modes[index].color[1],
        modes[index].color[2]
    );
    pixel->show();
}

int ToneController::getCurrentValue() const {
    return modes[this->currentModeIndex].currentValue;
}

String ToneController::getCurrentModeName() {
    return modes[this->currentModeIndex].name;
}

void ToneController::setCurrentValue(int value) {
    value = max(value, modes[this->currentModeIndex].minValue);
    value = min(value, modes[this->currentModeIndex].maxValue);
    int led_index = this->getMappedPixelIndex(value);
    modes[this->currentModeIndex].currentValue = value;

    pixel->clear();
    pixel->setFirstnPixelColor(
        led_index,
        modes[this->currentModeIndex].color[0],
        modes[this->currentModeIndex].color[1],
        modes[this->currentModeIndex].color[2]
    );
    pixel->show();
}

int ToneController::getMappedValue(int angle, int maxAngle) {
    int in_min = 0; // Minimum angle
    int in_max = maxAngle; // Maximum angle
    int out_min = modes[this->currentModeIndex].minValue;
    int out_max = modes[this->currentModeIndex].maxValue;
    if (angle < in_min) angle = in_min;
    else if (angle > in_max) return -1;
    return ceil(mapf(angle, in_min, in_max, out_min, out_max));
}

int ToneController::getMappedPixelIndex(int value) {
    float result = mapf(
        value,
        modes[this->currentModeIndex].minValue,
        modes[this->currentModeIndex].maxValue,
        0,
        this->_pixelCount + 1
    );
    return ceil(result);
}

void ToneController::sendDataChange() {
    const KVP data[5] = {
        {"mode", getCurrentModeName()},
        {"value", String(modes[this->currentModeIndex].currentValue)},
        {"r", String(modes[this->currentModeIndex].color[0])},
        {"g", String(modes[this->currentModeIndex].color[1])},
        {"b", String(modes[this->currentModeIndex].color[2])}
    };
    bluetooth->sendData(data, 5);
}

float ToneController::mapf(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}