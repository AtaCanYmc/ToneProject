#include <Arduino.h>
#include "ToneController.h"


#define NUMPIXELS 11        // NeoPixel LED sayısı
#define PIXELPIN  10       // GPIO10 - NeoPixel Data
#define XPIN      3        // GPIO3 - Joystick X
#define YPIN      4        // GPIO4 - Joystick Y
#define SWPIN     5        // GPIO5 - Joystick buton


/*
#define NUMPIXELS 13       // NeoPixel LED sayısı
#define PIXELPIN  13       // GPIO13 - NeoPixel Data
#define XPIN      34       // GPIO34 - Joystick X (ADC1)
#define YPIN      35       // GPIO35 - Joystick Y (ADC1)
#define SWPIN     25       // GPIO25 - Joystick buton
*/


ToneController tne(XPIN, YPIN, SWPIN, PIXELPIN, NUMPIXELS);

void setup() {
    Serial.begin(115200);
    tne.begin();

    // index, name, minValue, maxValue, r, g, b, brightness
    tne.setMode(0, "Volume", 0, 100, 255, 255, 155, 150);
    tne.setMode(1, "Bass", 0, 100, 122, 50, 245, 150);
    tne.setMode(2, "Treble", 0, 100, 90, 240, 255, 150);
    tne.setCurrentMode(0);

    delay(1000);
    Serial.println("ToneOS started");
}

void loop() {
    tne.update();
    delay(100);
}
