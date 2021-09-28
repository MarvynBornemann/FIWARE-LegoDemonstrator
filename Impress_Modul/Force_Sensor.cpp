#include "Force_Sensor.h"

Force_Sensor::Force_Sensor(int pinForceSensor1,int pinForceSensor2)
{
  this->pinForceSensor1 = pinForceSensor1;
  this->pinForceSensor2 = pinForceSensor2;
  this->alpha = 0.05;
}

Force_Sensor::Force_Sensor(int pinForceSensor1,int pinForceSensor2, float alpha)
{
  this->pinForceSensor1 = pinForceSensor1;
  this->pinForceSensor2 = pinForceSensor2;
  this->alpha = alpha;
}

void Force_Sensor::read()
{
    valueFS1 = analogRead(pinForceSensor1);
    valueFS2 = analogRead(pinForceSensor2);

    movingAvarageFS1 = (int) (alpha*valueFS1 + (1-alpha)*movingAvarageFS1);
    movingAvarageFS2 = (int) (alpha*valueFS2 + (1-alpha)*movingAvarageFS2);
}

int Force_Sensor::calculateAvarage()
{
    movingAvarageTotal = (int)(movingAvarageFS1 + movingAvarageFS2)/2;
    return movingAvarageTotal;
}

int Force_Sensor::getWeightClass()
{
    if(movingAvarageTotal < WEIGHT_CLASS_0){
        weightClass = 0;
    }else if(movingAvarageTotal < WEIGHT_CLASS_1){
        weightClass = 1;
    }else if(movingAvarageTotal < WEIGHT_CLASS_2){
        weightClass = 2;
    }else if(movingAvarageTotal < WEIGHT_CLASS_3){
        weightClass = 3;
    }else weightClass = 4;

    return weightClass;
}

int Force_Sensor::getPrice()
{   
    price = 9;
    //calculate Price
    if(weightClass == 1) price = 29;
    else if(weightClass == 2) price = 49;
    else if(weightClass == 3) price = 89;
    else if (weightClass == 4) price = 129;
    return price;
}

int Force_Sensor::getAvarageOfForceSensor1()
{
    return movingAvarageFS1;
}

int Force_Sensor::getAvarageOfForceSensor2()
{
    return movingAvarageFS2;
}


