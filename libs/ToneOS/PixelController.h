//
// Created by Ata Can Yaymacı on 25.04.2025.
//

#ifndef PIXELCONTROLLER_H
#define PIXELCONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/**
 * @brief A controller class for managing Adafruit NeoPixel LED strips or rings.
 * Provides easy manipulation of individual or grouped pixels, brightness, and effects.
 */
class PixelController {
private:
    Adafruit_NeoPixel *pixels;
    int currentPixel = -1; ///< Index of the currently selected pixel
    bool *pixelStatus;     ///< Stores pixel on/off status
    int numPixels;         ///< Total number of pixels
    int offset;            ///< Rotation offset for pixel layout
    bool isReverse;        ///< Is led index reversed

    /**
     * @brief Returns the true index of a pixel, considering the offset.
     * @param pixel Logical pixel index.
     * @return Mapped hardware pixel index.
     */
    int getPixelIndex(int pixel);

public:
/**
     * @brief Construct a new Pixel Controller object.
     * @param pin Digital pin connected to NeoPixel data input.
     * @param numPixels Number of pixels in the strip or ring.
     * @param offset Optional rotation offset (default is 0).
     * @param isReversed Optional reversed indexing (default is false).
     */
    PixelController(int pin, int numPixels, int offset = 0, bool isReverse=false);

    /**
     * @brief Initializes the NeoPixel strip.
     */
    void begin();

    /**
     * @brief Blinks all pixels with the specified color.
     * @param r Red component (0–255).
     * @param g Green component (0–255).
     * @param b Blue component (0–255).
     * @param delayTime Time in milliseconds for blink duration (default: 100ms).
     */
    void blink(int r, int g, int b, int delayTime = 100);

    /**
     * @brief Sets a specific pixel to a color using RGB values.
     * @param pixel Index of the pixel.
     * @param r Red (0–255).
     * @param g Green (0–255).
     * @param b Blue (0–255).
     */
    void setPixelColor(int pixel, int r, int g, int b);

    /**
     * @brief Sets a pixel to a 32-bit packed color.
     * @param pixel Index of the pixel.
     * @param color Packed RGB color (use Adafruit's Color method).
     */
    void setPixelColor(int pixel, uint32_t color);

    /**
     * @brief Sets the first N pixels to the given RGB color.
     * @param n Number of pixels to set.
     * @param r Red (0–255).
     * @param g Green (0–255).
     * @param b Blue (0–255).
     */
    void setFirstnPixelColor(int n, int r, int g, int b);

    /**
     * @brief Sets the brightness of the entire pixel strip.
     * @param brightness Brightness value (0–255).
     */
    void setBrightness(int brightness);

    /**
     * @brief Sets all pixels to the same color instantly.
     * @param r Red (0–255).
     * @param g Green (0–255).
     * @param b Blue (0–255).
     */
    void setAllPixelsColor(int r, int g, int b);

    /**
     * @brief Sets all pixels one by one with a delay for a wave-like effect.
     * @param r Red (0–255).
     * @param g Green (0–255).
     * @param b Blue (0–255).
     * @param delayTime Delay between each pixel update (ms, default: 10).
     */
    void setAllPixelsColorOrdered(int r, int g, int b, int delayTime = 10);

    /**
     * @brief Sets the currently active pixel (based on internal index) to a specific color.
     * @param r Red (0–255).
     * @param g Green (0–255).
     * @param b Blue (0–255).
     */
    void setCurrentPixelColor(int r, int g, int b);

    /**
     * @brief Applies all pending pixel changes to the strip (must be called to reflect changes).
     */
    void show();

    /**
     * @brief Clears all pixels (turns them off).
     */
    void clear();

    /**
     * @brief Returns the color of a specific pixel.
     * @param pixel Index of the pixel.
     * @return 32-bit packed RGB color.
     */
    uint32_t getPixelColor(int pixel);

    /**
     * @brief Returns the number of pixels controlled by this instance.
     * @return Total number of pixels.
     */
    int getNumPixels();
};

#endif //PIXELCONTROLLER_H
