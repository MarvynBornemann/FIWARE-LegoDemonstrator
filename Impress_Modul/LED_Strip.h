#pragma once

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

class LED_Strip
{
    private:
        long _lastTime;
        int pixelIndex;

        int mode;
        const int numberOfModes = 8;

        long rainbowFirstPixelHue = 0;
        int theaterChaseIndex = 0;

        int repeatIndex = 0;

        bool doOnce = true;
        int weldingWorkTime = 0;

        Adafruit_NeoPixel* strip;

    public:
        LED_Strip(int numberOfLEDs, int pinLED);
        ~LED_Strip();
        
        void setup();
        void loop();

        void setColor(uint8_t r, uint8_t g, uint8_t b);

        void plainColor(uint32_t color, int wait,int numberOfRepeat = 1);
        void colorWipe(uint32_t color, int wait, int numberOfRepeat = 1);
        void theaterChase(uint32_t color, int wait, int numberOfRepeat = 1);
        void rainbow(int wait, int numberOfRepeat = 1);
        void theaterChaseRainbow(int wait, int numberOfRepeat = 1);

        void weldingWork(int numberOfRepeat);

        bool nextPixel();
        bool repeat(int numberOfRepeat);
        void nextMode();
        void setMode(int mode);
};