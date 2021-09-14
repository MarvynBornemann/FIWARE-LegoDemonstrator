#include "Force_Sensor.h"

#define FORCE_SENSOR1_PIN 34
#define FORCE_SENSOR2_PIN 35

const float alpha = 0.05;
const int frequencyOfDisplayWeight = 1;
const int frequencyOfForceSensorReadings = 200;


int movingAvarageTotal = 0;

int weightClass = 0;

Force_Sensor forceSensor(FORCE_SENSOR1_PIN, FORCE_SENSOR2_PIN, alpha);


void setup() {
    Serial.begin(115200); 
}

void loop() {

    int numberOfForceSensorReadings = frequencyOfForceSensorReadings/frequencyOfDisplayWeight;
    for(int i = 0; i < numberOfForceSensorReadings; i++){
        forceSensor.read();
        delay(1000/frequencyOfForceSensorReadings);
    }

    movingAvarageTotal = forceSensor.calculateAvarage();

    weightClass = forceSensor.getWeightClass();

    // Serial.print("Sensor 1: ");
    // Serial.print(val_fs1);
    // Serial.print("   Sensor 2: ");
    // Serial.print(val_fs2);
    Serial.print("Classified: ");
    Serial.print(weightClass);
    Serial.print("   Mov_Avg Total: ");
    Serial.print(movingAvarageTotal);
    Serial.print("   Mov_Avg 1: ");
    Serial.print(forceSensor.getAvarageOfForceSensor1());
    Serial.print("   Mov_Avg 2: ");
    Serial.println(forceSensor.getAvarageOfForceSensor2());
}