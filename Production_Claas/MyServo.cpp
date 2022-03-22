#include "MyServo.h"

MyServo::MyServo(int PWMPin, bool continous){
    this->PWMPin = PWMPin;
    this->continous = continous;
}

void MyServo::attach(){
    servo.attach(PWMPin);
    stop();
}

void MyServo::detach(){
    stop();
    servo.detach();
}

//rotate continous servo (speed [0,90], clockwise [true, false])
void MyServo::rotate(int speed, bool clockwise){
    if(continous){
        //speed has to be between 0 and 90
        if(speed > 90) speed = 90;
        if(speed < 0) speed = 0;

        //turn servo clockwise or counter clockwise
        if(clockwise) servo.write(90 - speed);
        else servo.write(90 + speed);
    }
}

//stop continous servo
void MyServo::stop(){
    if(continous){
        servo.write(90); // 0 = turn clockwise, 90 = don't turn, 180 = turn counter clockwise
    }else servo.write(angle);
}

//not-continous servo 
bool MyServo::goToAngle(int wait, int degree, int stepSize){
    if(continous) return 0;

    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        int angleDiff = degree - angle;
        if(angleDiff > 0){
            if(angleDiff > stepSize)
                angle += stepSize;
            else angle = degree;
        }
        else if(angleDiff < 0){
            if(-angleDiff > stepSize)
                angle -= stepSize;
            else angle = degree;
        }
        else return 1;

        servo.write(angle);
    }
    return 0;
}