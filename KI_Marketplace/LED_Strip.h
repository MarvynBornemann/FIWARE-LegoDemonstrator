#pragma once

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

struct COLOR {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    COLOR() {
        r = 0; 
        g = 0; 
        b = 0;
    }
    COLOR(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class LED_Strip
{
    private:
        Adafruit_NeoPixel* strip;

        long _lastTime;
        int pixelIndex;

        long rainbowFirstPixelHue = 0;
        int theaterChaseIndex = 0;

        int repeatIndex = 0;

        COLOR color;
        COLOR colorMain;
        COLOR colorSecond;
        COLOR pixelColor;
        bool colorSecondIsSet = false;

        void chooseColorByIndex(int index);

        bool setStartPixel = true;

        bool colorWipeDoubleFinished1 = false;
        bool colorWipeDoubleFinished2 = false;

        bool nextPixel(bool direction = false, int startPixel = 0, int endPixel = -1);
        bool repeat(int numberOfRepeat);

    public:
        LED_Strip(int numberOfLEDs, int pinLED);
        ~LED_Strip();
        
        void setup();

        void setColor(uint8_t r, uint8_t g, uint8_t b);
        void setColor(COLOR color);
        void setColorSecond(COLOR color);
        int getNumberOfLEDs();
        void clear();

        bool plainColor(int wait, int numberOfRepeat = 1);
        bool colorWipe(int wait, bool direction = false, int startPixel = 0, int endPixel = -1, int numberOfRepeat = 1);
        bool colorWipeDouble(int wait, int startPixel, int endPixel1, int endPixel2, int numberOfRepeat = 1);
        bool colorWipeOneByOne(int wait, bool direction = false, int startPixel = 0, int endPixel = -1, int numberOfRepeat = 1);
        bool theaterChase(int wait, int numberOfRepeat = 1);
        bool rainbow(int wait, int numberOfRepeat = 1);
        bool rainbowPlainColor(int wait, int startPixel = 0, int numberOfPixels = -1, int numberOfRepeat = 1);
        bool theaterChaseRainbow(int wait, int numberOfRepeat = 1);
};