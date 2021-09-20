#pragma once

#include <Arduino.h>

#define LEDC_CHANNEL_0 0
#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000

class LED_PWM
{
    private:
        int pinLED;

        int brightness = 0;
        int fadeAmount = 5;

        const int frequenzy = 30;
        long _lastTime = 0;

    public:
        LED_PWM(int pinLED);

        void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255);
        void setup();
        void loop();
};