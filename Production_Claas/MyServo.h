#pragma once

#include <Arduino.h>

#include <Servo.h>

class MyServo
{
    private:
        long _lastTime;

        Servo servo;

        bool continous = false;
        int PWMPin = 0;

        int angle = 0;
        
    public:
        MyServo(int PWMPin, bool continous = false);

        void attach();
        void detach();
        void rotate(int speed, bool clockwise = true);
        void stop();
        bool goToAngle(int wait, int degree, int stepSize = 1);
};