#pragma once

#include <Arduino.h>

#include "LED_Strip.h"

class RGB_LED
{
    private:
        long _lastTime;
        int repeatIndex = 0;

        int pinR;
        int pinG;
        int pinB;

        COLOR color;

        int step = 0;
        bool stepDirection = false;

        int weldingWorkTime = 1000;

        bool repeat(int numberOfRepeat);

        void writeColor(COLOR _color);

    public:
        RGB_LED(int pinR, int pinG, int pinB);

        void setup();
        void clear();

        void setColor(COLOR color);

        bool plainColor(int wait, int numberOfRepeat = 1);

        bool blinkPlainColor(int wait, int numberOfSteps = 10, int numberOfRepeat = 1);

        bool weldingWork(int numberOfRepeat);
};