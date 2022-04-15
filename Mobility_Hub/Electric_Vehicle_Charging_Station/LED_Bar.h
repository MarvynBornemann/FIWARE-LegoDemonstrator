#pragma once

#include <Arduino.h>

#include <Grove_LED_Bar.h>

#define NUMBER_OF_LEVELS 10

#define TIME_PER_LEVEL_MIN 1000
#define TIME_PER_LEVEL_MAX 10000

class LED_Bar
{
    private:
        long _lastTime;

        Grove_LED_Bar* ledBar;

        int level = 0;

        long timePerLevel = 0;
    public:
        LED_Bar(int clockPin, int dataPin);

        void setup();

        int charging(long timePerLevel);
        void reset();
};