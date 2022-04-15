#include "LED_Bar.h"

LED_Bar::LED_Bar(int clockPin, int dataPin){
    ledBar = new Grove_LED_Bar(clockPin, dataPin, 0, LED_BAR_10);
}

void LED_Bar::setup(){
    ledBar->begin();
}

int LED_Bar::charging(long timePerLevel){
    long _currentTime = millis();
    if(_currentTime - _lastTime > timePerLevel) {
        _lastTime = _currentTime;

        if(level < NUMBER_OF_LEVELS){
            level++;
        }
        ledBar->setLevel(level);
    }
    return level;
}

void LED_Bar::reset(){
    level = 0;
    ledBar->setLevel(level);
}