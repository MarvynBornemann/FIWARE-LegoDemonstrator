#pragma once

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#define LED_BRIGHTNESS 50       // Set brightness of LED strip (max = 255)

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
        int repeatIndex = 0;

        int dataIndex;

        COLOR color;

        bool setStartPixel = true;

        bool nextPixel(bool direction = false, int startPixel = 0, int endPixel = -1);
        bool repeat(int numberOfRepeat);

    public:
        LED_Strip(int numberOfLEDs, int pinLED);
        ~LED_Strip();
        
        void setup();
        void clear();

        void setColor(COLOR color);
        int getNumberOfLEDs();

        bool plainColor(int wait, int numberOfRepeat = 1);
        bool colorWipe(int wait, bool direction = false, int startPixel = 0, int endPixel = -1, int numberOfRepeat = 1);
        bool colorWipeOneByOne(int wait, bool direction = false, int startPixel = 0, int endPixel = -1, int numberOfRepeat = 1);
        bool colorWipeOneByOne_doubleStrip(int wait, bool direction = false, int startPixel = 0, int endPixel = -1, int numberOfRepeat = 1);
        bool dataWipe(int wait, const bool datastream[], int datastreamLength, int numberOfRepeat = 1);
};