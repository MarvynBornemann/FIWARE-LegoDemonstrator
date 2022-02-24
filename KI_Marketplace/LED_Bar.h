#pragma once

#include <Arduino.h>

#include <Grove_LED_Bar.h>

#define NUMBER_OF_LEVELS 10

#define TIME_PER_LEVEL_MIN 1000
#define TIME_PER_LEVEL_MAX 10000

#define TIME_UNTIL_DISCONNECT_MIN 10000
#define TIME_UNTIL_DISCONNECT_MAX 20000

#define TIME_UNTIL_NEXT_CHARGING_MIN 20000
#define TIME_UNTIL_NEXT_CHARGING_MAX 60000

class LED_Bar
{
    private:
        long _lastTime;

        Grove_LED_Bar* ledBar;

        int level = 0;

        int stage = 0;

        long timePerLevel = 0;
        long timeUntilDisconnect = 0;
        long timeUntilNextCharging = 0;
    public:
        LED_Bar(int clockPin, int dataPin);

        void setup();
        void loop();

};