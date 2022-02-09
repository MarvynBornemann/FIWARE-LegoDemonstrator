#pragma once

#include <Arduino.h>

#include "LED_Strip.h"

//LED Strips
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

const COLOR colorLEDStrip1(255,0,0);
const COLOR colorLEDStrip2(0,255,0);
const COLOR colorLEDStrip3(0,0,255);
const COLOR colorLEDStrip4(255,0,255);
const COLOR colorLEDStrip5(255,255,0);

//LED Ring
#define LED_RING_PIN 2
#define NUMBER_OF_LEDS_RING 12

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
        LED_Strip* ledStrips[5] = {ledStrip1,ledStrip2,ledStrip3,ledStrip4,ledStrip5};

        COLOR colorFirst;
        COLOR colorSecond;
        COLOR colorMix;
        COLOR colors[5] = {colorLEDStrip1,colorLEDStrip2,colorLEDStrip3,colorLEDStrip4,colorLEDStrip5};

        int simulationStage = 0;
        int simulationIndex = 0;
        bool simulationFinished = true;

        void nextSimulationStage(bool ledStripFinished);
        void getRandomStrips();
        COLOR mix2Colors(COLOR color1, COLOR color2); 

        void simulation0();
        void simulation1();

        void clearAllStrips();

    public:
        LED_Simulation();

        void setup();
        void loop();

        void nextSimulation(int simulationIndex);
};