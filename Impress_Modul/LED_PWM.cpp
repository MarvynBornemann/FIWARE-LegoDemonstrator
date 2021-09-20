#include "LED_PWM.h"

LED_PWM::LED_PWM(int pinLED){
    this->pinLED = pinLED;
}

void LED_PWM::ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax)
{
    uint32_t duty = (8191 / valueMax) * min(value, valueMax);
    ledcWrite(channel, duty);
}

void LED_PWM::setup() {
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(pinLED, LEDC_CHANNEL_0);
}

void LED_PWM::loop() {
    long _currentTime = millis();
    if(_currentTime - _lastTime > frequenzy) {
        _lastTime = _currentTime;

        ledcAnalogWrite(LEDC_CHANNEL_0, brightness);
        brightness = brightness + fadeAmount;
        if (brightness <= 0 || brightness >= 255) {
            fadeAmount = -fadeAmount;
        }
    }
}