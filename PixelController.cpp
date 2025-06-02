//
// Created by Ata Can Yaymacı on 25.04.2025.
//


#include "PixelController.h"

PixelController::PixelController(int pin, int numPixels, int offset, bool isReverse) : numPixels(numPixels), offset(offset), isReverse(isReverse) {
    this->pixelStatus = new bool[numPixels];
    this->offset = offset;
    this->isReverse = isReverse;
    this->numPixels = numPixels;
    this->pixels = new Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);
    this->currentPixel = -1;
    this->pixels->setBrightness(255); // Set default brightness to maximum

    for (int i = 0; i < numPixels; i++) {
        pixelStatus[i] = false;
    }
}

void PixelController::begin() {
    pixels->begin();
    pixels->show();
}

void PixelController::blink(int r, int g, int b, int delayTime) {
    this->setAllPixelsColor(r, g, b);
    this->show();
    delay(delayTime);
    this->setAllPixelsColor(0, 0, 0);
    this->show();
}

void PixelController::setPixelColor(int pixel, int r, int g, int b) {
    pixel = getPixelIndex(pixel);
    if (pixel >= 0 && pixel < numPixels) {
        pixels->setPixelColor(pixel, pixels->Color(r, g, b));
        pixelStatus[pixel] = true;
    }
}

void PixelController::setPixelColor(int pixel, uint32_t color) {
    pixel = getPixelIndex(pixel);
    if (pixel >= 0 && pixel < numPixels) {
        pixels->setPixelColor(pixel, color);
        pixelStatus[pixel] = true;
    }
}

int PixelController::getPixelIndex(int pixel) {
    pixel = pixel + offset;
    if (pixel < 0) pixel = numPixels + pixel;
    if (pixel > numPixels) pixel = pixel % numPixels;
    if (this->isReverse) pixel = (numPixels - pixel) - 1;
    return pixel;
}

void PixelController::setFirstnPixelColor(int n, int r, int g, int b) {
    for (int i = 0; i < n; i++) {
        setPixelColor(i, r, g, b);
    }
    this->currentPixel = n;
}

void PixelController::setBrightness(int brightness) {
    pixels->setBrightness(brightness);
}

void PixelController::setAllPixelsColor(int r, int g, int b) {
    for (int i = 0; i < numPixels; i++) {
        setPixelColor(i, r, g, b);
    }
}

void PixelController::setAllPixelsColorOrdered(int r, int g, int b, int delayTime) {
    for (int i = 0; i < numPixels; i++) {
        setPixelColor(i, r, g, b);
        this->show();
        delay(delayTime);
    }
}

void PixelController::setCurrentPixelColor(int r, int g, int b) {
    if (currentPixel >= 0 && currentPixel < numPixels) {
        setPixelColor(currentPixel, r, g, b);
    }
}

void PixelController::show() {
    pixels->show();
}

void PixelController::clear() {
    for (int i = 0; i < numPixels; i++) {
        pixels->setPixelColor(i, pixels->Color(0, 0, 0));
        pixelStatus[i] = false;
    }
    pixels->show();
}

uint32_t PixelController::getPixelColor(int pixel) {
    if (pixel >= 0 && pixel < numPixels) {
        return pixels->getPixelColor(pixel);
    }
    return 0;
}

int PixelController::getNumPixels() {
    return numPixels;
}

// PixelController.h
