#include "RGB_LED.h"

RGB_LED::RGB_LED(int pinR, int pinG, int pinB){
    this->pinR = pinR;
    this->pinG = pinG;
    this->pinB = pinB;
}

void RGB_LED::setup(){
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);
}

void RGB_LED::clear(){
    digitalWrite(pinR, LOW);
    digitalWrite(pinG, LOW);
    digitalWrite(pinB, LOW);
}

//return 1 if this function got executed #numberOfRepeat times
bool RGB_LED::repeat(int numberOfRepeat){
    repeatIndex++;
    if(repeatIndex >= numberOfRepeat){
        repeatIndex = 0;
        return 1;
    }
    return 0;
}

// set color of RGB LED (COLOR)
void RGB_LED::setColor(COLOR color) {
    this->color = color;
}

void RGB_LED::writeColor(COLOR _color){
    analogWrite(pinR, _color.r);
    analogWrite(pinG, _color.g);
    analogWrite(pinB, _color.b);
}

// Show RGB LED with plain Color.
// Pass delay time (in ms) between frames.
bool RGB_LED::plainColor(int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        writeColor(color);

        if(repeat(numberOfRepeat)) return 1;
    }
    return 0;
}

bool RGB_LED::blinkPlainColor(int wait, int numberOfSteps, int numberOfRepeat){
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        float divStep = (step * 1.0) / numberOfSteps;

        writeColor(COLOR((int)(color.r * divStep), (int)(color.g * divStep), (int)(color.b * divStep)));

        if(stepDirection) step--;
        else step++;

        if(step >= numberOfSteps) stepDirection = true;

        if(step < 0){
            step = 0;
            stepDirection = false;
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}

bool RGB_LED::weldingWork(int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > weldingWorkTime) {
        _lastTime = _currentTime;
        weldingWorkTime = random(20,100);
        
        int rand = random(0,10);
        if(rand>8){
            writeColor(COLOR(random(200,255),random(200,255),0));
        }else if(rand>5){
            writeColor(COLOR(0,random(0,255),random(200,255)));
        }else if(rand>4){
            writeColor(COLOR(random(0,100),0,random(200,255)));
        }else if(rand>2){
            writeColor(COLOR(255,255,255));
        }else{
            clear();
        }

        if(repeat(numberOfRepeat)){
            return 1;
        }
    }
    return 0;
}