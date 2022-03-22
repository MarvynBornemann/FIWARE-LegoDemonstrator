#pragma once

#include <Arduino.h>

#include "LED_Strip.h"
#include "MyServo.h"
#include "RGB_LED.h"

//Number of Simuations
#define NUMBER_OF_SIMULATIONS 2

//LED Strips
#define LED_STRIP1_PIN 1
#define NUMBER_OF_LEDS_STRIP1 28

#define LED_STRIP2_PIN 3
#define NUMBER_OF_LEDS_STRIP2 21

//Servo
#define SERVO1_PIN 15
#define SERVO2_PIN 13
#define SERVO3_PIN 12

//RGB LEDs
#define GREEN_LED1_PIN 14
#define RED_LED1_PIN 2
#define BLUE_LED1_PIN 0
#define BLUE_LED2_PIN 4
#define RED_LED2_PIN 5
#define GREEN_LED2_PIN 16


//Frequencies
#define TIME_OF_COLOR_WIPE 100
#define TIME_OF_DATASTREAM 50
#define TIME_OF_BLINK 50
#define STEPS_OF_BLINK 15
#define TIME_OF_PAUSE 1000
#define TIME_OF_SERVO 20


//datastream for ledStrip
const int datastreamLength = 64;
const bool datastream[datastreamLength] =  {1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,1,0,1,1,1,
                                            1,0,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,1};

//maximum number of different stage parts to finish at one stage
#define NUMBER_OF_STAGE_PARTS_MAXIMUM 5

class Simulation
{
    private:
        //LED Strips
        LED_Strip* ledStripScanner;
        LED_Strip* ledStripDatastream;

        //RGB LEDs
        RGB_LED* rgbLED1;
        RGB_LED* rgbLED2;

        //Servos
        MyServo* servoScanner;
        MyServo* servoRobot1;
        MyServo* servoRobot2;

        bool stagePartFinished[NUMBER_OF_STAGE_PARTS_MAXIMUM] = {false};

        //Simulation Stage Managment
        int simulationStage = 0;
        int simulationIndex = 1;
        bool simulationFinished = true;

        void nextSimulationStage(bool ledStripFinished);

        //Simulations
        void simulation1();
        void simulation2();

        //turn everything off
        void turnEverythingOff();

        //Pause
        long pauseStartTime;
        bool setPauseStartTime = true;
        bool pause(long milliseconds);

        //servos
        void attachServos();
        void detachServos();

    public:
        Simulation();

        void setup();
        void loop();

        void nextSimulation(int simulationIndex);
};