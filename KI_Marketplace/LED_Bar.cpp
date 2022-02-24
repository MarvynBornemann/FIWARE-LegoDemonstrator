#include "LED_Bar.h"

LED_Bar::LED_Bar(int clockPin, int dataPin){
    ledBar = new Grove_LED_Bar(clockPin, dataPin, 0, LED_BAR_10);
}

void LED_Bar::setup(){
    ledBar->begin();
    timePerLevel = random(TIME_PER_LEVEL_MIN, TIME_PER_LEVEL_MAX);
}

void LED_Bar::loop(){
    if(stage == 0){
        long _currentTime = millis();
        if(_currentTime - _lastTime > timePerLevel) {
            _lastTime = _currentTime;

            ledBar->setLevel(level);

            level++;
            if(level > NUMBER_OF_LEVELS){
                stage = 1;
                timeUntilDisconnect = random(TIME_UNTIL_DISCONNECT_MIN, TIME_UNTIL_DISCONNECT_MAX);
            }
        }
    }
    else if(stage == 1){
        long _currentTime = millis();
        if(_currentTime - _lastTime > timeUntilDisconnect) {
            _lastTime = _currentTime;

            ledBar->setLevel(0);
            
            timeUntilNextCharging = random(TIME_UNTIL_NEXT_CHARGING_MIN, TIME_UNTIL_NEXT_CHARGING_MAX);
            stage = 2;
        }
    }
    else if(stage == 2){
        long _currentTime = millis();
        if(_currentTime - _lastTime > timeUntilNextCharging) {
            _lastTime = _currentTime;

            level = 0;
            stage = 0;
            timePerLevel = random(TIME_PER_LEVEL_MIN, TIME_PER_LEVEL_MAX);
        }
    }
    
}