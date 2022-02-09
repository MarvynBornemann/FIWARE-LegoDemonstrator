#pragma once

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#define NUMBER_OF_LED_MODES 9

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

        int mode;
        const int numberOfModes = NUMBER_OF_LED_MODES;

        long rainbowFirstPixelHue = 0;
        int theaterChaseIndex = 0;

        int repeatIndex = 0;

        bool doOnce = true;
        int weldingWorkTime = 0;

        COLOR color;

        bool nextPixel();
        bool repeat(int numberOfRepeat);

    public:
        LED_Strip(int numberOfLEDs, int pinLED);
        ~LED_Strip();
        
        void setup();
        void loop();

        void setColor(uint8_t r, uint8_t g, uint8_t b);
        void setColor(COLOR color);
        void clear();

        bool plainColor(int wait,int numberOfRepeat = 1);
        bool colorWipe(int wait, int numberOfRepeat = 1);
        bool theaterChase(int wait, int numberOfRepeat = 1);
        bool rainbow(int wait, int numberOfRepeat = 1);
        bool theaterChaseRainbow(int wait, int numberOfRepeat = 1);

        void nextMode();
        void setMode(int mode);
};