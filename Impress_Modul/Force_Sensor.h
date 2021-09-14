#pragma once

#include <Arduino.h>

//Maximum force sensor values for each weight class
#define WEIGHT_CLASS_0 50
#define WEIGHT_CLASS_1 750
#define WEIGHT_CLASS_2 1400
#define WEIGHT_CLASS_3 2000

class Force_Sensor
{
    private:
        int pinForceSensor1; //Pin of Force Sensor 1
        int pinForceSensor2; //Pin of Force Sensor 2

        int valueFS1 = 0;
        int valueFS2 = 0;

        float alpha; //moving avarage parameter
        int movingAvarageFS1 = 0;
        int movingAvarageFS2 = 0;
        int movingAvarageTotal = 0;

        int weightClass = 0;

    public:
        Force_Sensor(int pinForceSensor1,int pinForceSensor2);
        Force_Sensor(int pinForceSensor1,int pinForceSensor2, float alpha);
        void read();
        int calculateAvarage();
        int getWeightClass();
        int getAvarageOfForceSensor1();
        int getAvarageOfForceSensor2();
};