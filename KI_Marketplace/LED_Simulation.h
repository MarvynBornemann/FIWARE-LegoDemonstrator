#pragma once

#include <Arduino.h>

#include "LED_Strip.h"

//Number of Simuations
#define NUMBER_OF_LED_SIMULATIONS 3

//LED Strips
#define NUMBER_OF_LED_STRIPS 5
#define NUMBER_OF_ALL_LED_STRIPS 7

#define LED_STRIP1_PIN 1
#define NUMBER_OF_LEDS_STRIP1 7

#define LED_STRIP2_PIN 3
#define NUMBER_OF_LEDS_STRIP2 7

#define LED_STRIP3_PIN 15
#define NUMBER_OF_LEDS_STRIP3 7

#define LED_STRIP4_PIN 13
#define NUMBER_OF_LEDS_STRIP4 7

#define LED_STRIP5_PIN 12
#define NUMBER_OF_LEDS_STRIP5 7

#define LED_STRIP6_PIN 14
#define NUMBER_OF_LEDS_STRIP6 7

//good online tool to choose colors
//https://acrylgiessen.com/farben-mischen-online/

// const COLOR colorLEDStrip1(255,0,0);
// const COLOR colorLEDStrip2(204,255,0);
// const COLOR colorLEDStrip3(0,255,102);
// const COLOR colorLEDStrip4(0,102,255);
// const COLOR colorLEDStrip5(204,0,255);

const COLOR colorLEDStrip1(255,43,0);
const COLOR colorLEDStrip2(161,255,0);
const COLOR colorLEDStrip3(0,255,145);
const COLOR colorLEDStrip4(0,59,255);
const COLOR colorLEDStrip5(246,0,255);

const int ringIndexLEDStrip1 = 1;
const int ringIndexLEDStrip2 = 3;
const int ringIndexLEDStrip3 = 5;
const int ringIndexLEDStrip4 = 7;
const int ringIndexLEDStrip5 = 9;
const int ringIndexLEDStrip6 = 11;

//LED Ring
#define LED_RING_PIN 2
#define NUMBER_OF_LEDS_RING 12

//Frequencies
#define TIME_OF_COLOR_WIPE 50
#define TIME_OF_RAINBOW 10

#define SIMULATION1_REPEATS 3

class LED_Simulation
{
    private:
        //LED Strips
        LED_Strip* ledStrip1;
        LED_Strip* ledStrip2;
        LED_Strip* ledStrip3;
        LED_Strip* ledStrip4;
        LED_Strip* ledStrip5;
        LED_Strip* ledStrip6;
        LED_Strip* ledRing;

        LED_Strip* ledStripFirst;
        LED_Strip* ledStripSecond;
        LED_Strip* ledStrips[NUMBER_OF_ALL_LED_STRIPS];

        bool ledStripsFinished[NUMBER_OF_ALL_LED_STRIPS] = {false};
        bool allLEDStripsFinished = false;

        COLOR colorFirst;
        COLOR colorSecond;
        COLOR colorMix;
        COLOR colors[NUMBER_OF_LED_STRIPS] = {colorLEDStrip1,colorLEDStrip2,colorLEDStrip3,colorLEDStrip4,colorLEDStrip5};

        int ringIndexFirst;
        int ringIndexSecond;
        int ringIndices[NUMBER_OF_LED_STRIPS] = {ringIndexLEDStrip1,ringIndexLEDStrip2,ringIndexLEDStrip3,ringIndexLEDStrip4,ringIndexLEDStrip5};

        int simulationStage = 0;
        int simulationIndex = 0;
        bool simulationFinished = true;

        void nextSimulationStage(bool ledStripFinished);
        void getRandomStrips();
        COLOR mix2Colors(COLOR color1, COLOR color2);
        bool ledRing_colorWipeShortestWay(int ringIndexStart, int ringIndexEnd);        

        int counterSimulation1 = 0;
        void simulation1();
        void simulation2();

        int counterSimulation3 = 0;
        void simulation3(COLOR color);

        int counterSimulation4 = 0;
        void simulation4();

        void clearAllStrips();


        long pauseStartTime;
        bool setPauseStartTime = true;
        bool pause(long milliseconds);

    public:
        LED_Simulation();

        void setup();
        void loop();

        void nextSimulation(int simulationIndex);
};