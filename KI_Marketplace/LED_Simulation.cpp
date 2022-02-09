#include "LED_Simulation.h"

LED_Simulation::LED_Simulation(){
    ledStrip1 = new LED_Strip(NUMBER_OF_LEDS_STRIP1, LED_STRIP1_PIN);
    ledStrip2 = new LED_Strip(NUMBER_OF_LEDS_STRIP2, LED_STRIP2_PIN);
    ledStrip3 = new LED_Strip(NUMBER_OF_LEDS_STRIP3, LED_STRIP3_PIN);
    ledStrip4 = new LED_Strip(NUMBER_OF_LEDS_STRIP4, LED_STRIP4_PIN);
    ledStrip5 = new LED_Strip(NUMBER_OF_LEDS_STRIP5, LED_STRIP5_PIN);
    ledStrip6 = new LED_Strip(NUMBER_OF_LEDS_STRIP6, LED_STRIP6_PIN);
    ledRing = new LED_Strip(NUMBER_OF_LEDS_RING, LED_RING_PIN);

    simulationStage = 0;
    simulationIndex = 0;
    simulationFinished = true;
}

void LED_Simulation::nextSimulationStage(bool ledStripFinished){
    if(ledStripFinished){
        simulationStage++;
    }
}

void LED_Simulation::nextSimulation(int simulationIndex){
    this->simulationIndex = simulationIndex;
    simulationStage = 0;
    simulationFinished = true;
    clearAllStrips();
}

void LED_Simulation::clearAllStrips(){
    ledStrip1->clear();
    ledStrip2->clear();
    ledStrip3->clear();
    ledStrip4->clear();
    ledStrip5->clear();
    ledStrip6->clear();
    ledRing->clear();
}

void LED_Simulation::setup(){
    ledStrip1->setup();
    ledStrip2->setup();
    ledStrip3->setup();
    ledStrip4->setup();
    ledStrip5->setup();
    ledStrip6->setup();
    ledRing->setup();
}

void LED_Simulation::loop(){
    switch(simulationIndex){
        case 0: 
            simulation0();
            break;
        case 1:
            simulation1();
            break;
        default:
    }
}

void LED_Simulation::simulation0(){
    if(simulationFinished){
        simulationFinished = false;
        getRandomStrips();
    }
    switch(simulationStage){
        case 0:
            ledStripFirst->setColor(colorFirst);
            bool ledStripFinished = ledStripFirst->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 1:
            ledRing->setColor(colorFirst);
            bool ledStripFinished = ledRing->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 2:
            ledStrip6->setColor(colorFirst);
            bool ledStripFinished = ledStrip6->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 3:
            ledStripSecond->setColor(colorSecond);
            bool ledStripFinished = ledStripSecond->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 4:
            ledRing->setColor(colorSecond);
            bool ledStripFinished = ledRing->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 5:
            ledStrip6->setColor(colorSecond);
            bool ledStripFinished = ledStrip6->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 6:
            ledStrip6->setColor(colorMix);
            bool ledStripFinished = ledStrip6->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 7:
            ledRing->setColor(colorMix);
            bool ledStripFinished = ledRing->colorWipe(50);
            nextSimulationStage(ledStripFinished);
            break;
        case 8:
            ledStripFirst->setColor(colorMix);
            ledStripSecond->setColor(colorMix);
            bool ledStripFinished1 = ledStripFirst->colorWipe(50);
            bool ledStripFinished2 = ledStripSecond->colorWipe(50);
            nextSimulationStage(ledStripFinished1 && ledStripFinished2);
            break;
        case 9:
            nextSimulation(0);
            break;
        default:
    }
    
}

void LED_Simulation::getRandomStrips(){
    //choose two random index
    int indexFirst = random(0,4);
    int indexSecond = random(0,4);
    int t = 0;
    while(indexFirst == indexSecond && t<10){
        indexSecond = random(0,4);
        t++;
    }
    if(indexFirst == indexSecond) indexSecond = indexFirst + 1;
    if(indexSecond >= 5) indexSecond = 0;

    //write colors
    colorFirst = colors[indexFirst];
    colorSecond = colors[indexSecond];
    colorMix = mix2Colors(colorFirst, colorSecond);

    ledStripFirst = ledStrips[indexFirst];
    ledStripSecond = ledStrips[indexSecond];
}

COLOR LED_Simulation::mix2Colors(COLOR color1, COLOR color2){
    return COLOR((color1.r + color2.r)/2, (color1.g + color2.g)/2, (color1.b + color2.b)/2);
} 
