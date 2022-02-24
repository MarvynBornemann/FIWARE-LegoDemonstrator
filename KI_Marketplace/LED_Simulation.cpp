#include "LED_Simulation.h"

LED_Simulation::LED_Simulation(){
    ledStrip1 = new LED_Strip(NUMBER_OF_LEDS_STRIP1, LED_STRIP1_PIN);
    ledStrip2 = new LED_Strip(NUMBER_OF_LEDS_STRIP2, LED_STRIP2_PIN);
    ledStrip3 = new LED_Strip(NUMBER_OF_LEDS_STRIP3, LED_STRIP3_PIN);
    ledStrip4 = new LED_Strip(NUMBER_OF_LEDS_STRIP4, LED_STRIP4_PIN);
    ledStrip5 = new LED_Strip(NUMBER_OF_LEDS_STRIP5, LED_STRIP5_PIN);
    ledStrip6 = new LED_Strip(NUMBER_OF_LEDS_STRIP6, LED_STRIP6_PIN);
    ledRing = new LED_Strip(NUMBER_OF_LEDS_RING, LED_RING_PIN);

    ledStrips[0] = ledStrip1;
    ledStrips[1] = ledStrip2;
    ledStrips[2] = ledStrip3;
    ledStrips[3] = ledStrip4;
    ledStrips[4] = ledStrip5;
    ledStrips[5] = ledStrip6;
    ledStrips[6] = ledRing;

    simulationStage = 0;
    simulationIndex = 1;
    simulationFinished = true;
}

void LED_Simulation::nextSimulationStage(bool ledStripFinished){
    if(ledStripFinished){
        simulationStage++;
        for(int i = 0; i < NUMBER_OF_ALL_LED_STRIPS; i++){
            ledStripsFinished[i] = false;
        }
    }
}

void LED_Simulation::nextSimulation(int simulationIndex){
    this->simulationIndex = simulationIndex;
    simulationStage = 0;
    simulationFinished = true;
    clearAllStrips();
}

bool LED_Simulation::pause(long milliseconds){
    if(setPauseStartTime){
        setPauseStartTime = false;
        pauseStartTime = millis();
    }
    long currentTime = millis();
    if(currentTime - pauseStartTime > milliseconds) {
        setPauseStartTime = true;
        return 1;
    }
    return 0;
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
            clearAllStrips();
            break;
        case 1:
            if(counterSimulation1 >= SIMULATION1_REPEATS){
                counterSimulation1 = 0;
                nextSimulation(2);
                break;
            }
            simulation1();
            break;
        case 2:
            simulation2();
            break;
        case 3:
            if(counterSimulation3 >= NUMBER_OF_LED_STRIPS){
                counterSimulation3 = 0;
                nextSimulation(1);
                break;
            }
            simulation3(colors[counterSimulation3]);
            break;
        default:
            clearAllStrips();
            break;
    }
}

void LED_Simulation::simulation1(){
    //do only at beginning of simulation
    if(simulationFinished){
        simulationFinished = false;
        getRandomStrips();
    }
    switch(simulationStage){
        case 0:
            ledStripFirst->setColor(colorFirst);
            ledStripsFinished[0] = ledStripFirst->colorWipe(TIME_OF_COLOR_WIPE, true, ledStripFirst->getNumberOfLEDs() - 1);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 1:
            ledRing->setColor(colorFirst);
            ledStripsFinished[0] = ledRing_colorWipeShortestWay(ringIndexFirst, ringIndexLEDStrip6);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 2:
            ledStrip6->setColor(colorFirst);
            ledStripsFinished[0] = ledStrip6->colorWipe(TIME_OF_COLOR_WIPE);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 3:
            nextSimulationStage(pause(TIME_OF_COLOR_WIPE * 10));
            break;
        case 4:
            clearAllStrips();
            nextSimulationStage(pause(TIME_OF_COLOR_WIPE));
            break;
        case 5:
            ledStripSecond->setColor(colorSecond);
            ledStripsFinished[0] = ledStripSecond->colorWipe(TIME_OF_COLOR_WIPE, true, ledStripSecond->getNumberOfLEDs() - 1);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 6:
            ledRing->setColor(colorSecond);
            ledStripsFinished[0] = ledRing_colorWipeShortestWay(ringIndexSecond, ringIndexLEDStrip6);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 7:
            ledStrip6->setColor(colorSecond);
            ledStripsFinished[0] = ledStrip6->colorWipe(TIME_OF_COLOR_WIPE);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 8:
            nextSimulationStage(pause(TIME_OF_COLOR_WIPE * 10));
            break;
        case 9:
            clearAllStrips();
            nextSimulationStage(pause(TIME_OF_COLOR_WIPE));
            break;
        case 10:
            ledStrip6->setColor(colorMix);
            ledStripsFinished[0] = ledStrip6->colorWipe(TIME_OF_COLOR_WIPE, true, ledStrip6->getNumberOfLEDs() - 1);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 11:
            ledRing->setColor(colorMix);
            ledStripsFinished[0] = ledRing->colorWipeDouble(TIME_OF_COLOR_WIPE, ringIndexLEDStrip6, ringIndexFirst, ringIndexSecond);
            nextSimulationStage(ledStripsFinished[0]);
            break;
        case 12:
            if(!ledStripsFinished[0]){
                ledStripFirst->setColor(colorMix);
                ledStripsFinished[0] = ledStripFirst->colorWipe(TIME_OF_COLOR_WIPE);
            } 
            if(!ledStripsFinished[1]){
                ledStripSecond->setColor(colorMix);
                ledStripsFinished[1] = ledStripSecond->colorWipe(TIME_OF_COLOR_WIPE);
            } 
            nextSimulationStage((ledStripsFinished[0] && ledStripsFinished[1]));
            break;
        case 13:
            nextSimulationStage(pause(2000));
            break;
        case 14:
            clearAllStrips();
            nextSimulationStage(pause(1000));
            break;
        case 15:
            counterSimulation1++;
            nextSimulation(1);
            break;
        default:
            clearAllStrips();
            break;
    }
    
}

void LED_Simulation::simulation2(){
    //do only at beginning of simulation
    if(simulationFinished){
        simulationFinished = false;
    }

    switch(simulationStage){
        case 0:
            allLEDStripsFinished = true;
            for(int i = 0; i < NUMBER_OF_ALL_LED_STRIPS - 1; i++){
                if(!ledStripsFinished[i]){
                    ledStripsFinished[i] = ledStrips[i]->rainbow(TIME_OF_RAINBOW);
                    allLEDStripsFinished = (allLEDStripsFinished && ledStripsFinished[i]);
                }
            }
            if(!ledStripsFinished[6]){
                ledStripsFinished[6] = ledRing->rainbowPlainColor(TIME_OF_RAINBOW, ledStrip1->getNumberOfLEDs() - 1, ledStrip1->getNumberOfLEDs());
                allLEDStripsFinished = (allLEDStripsFinished && ledStripsFinished[6]);
            }
            nextSimulationStage(allLEDStripsFinished);
            break;
        case 1:
            clearAllStrips();
            nextSimulationStage(pause(TIME_OF_COLOR_WIPE*20));
            break;
        case 2:
            nextSimulation(3);
            break;
        default:
            clearAllStrips();
            break;
    }
}

void LED_Simulation::getRandomStrips(){
    //choose two random index
    int indexFirst = random(0,NUMBER_OF_LED_STRIPS);
    int indexSecond = random(0,NUMBER_OF_LED_STRIPS);
    int t = 0;
    while(indexFirst == indexSecond && t<10){
        indexSecond = random(0, NUMBER_OF_LED_STRIPS);
        t++;
    }
    if(indexFirst == indexSecond) indexSecond = indexFirst + 1;
    if(indexSecond >= NUMBER_OF_LED_STRIPS) indexSecond = 0;

    //get colors
    colorFirst = colors[indexFirst];
    colorSecond = colors[indexSecond];
    colorMix = mix2Colors(colorFirst, colorSecond);

    //get ring indices
    ringIndexFirst = ringIndices[indexFirst];
    ringIndexSecond = ringIndices[indexSecond];

    //get led strips
    ledStripFirst = ledStrips[indexFirst];
    ledStripSecond = ledStrips[indexSecond];
}

COLOR LED_Simulation::mix2Colors(COLOR color1, COLOR color2){
    return COLOR((color1.r + color2.r)/2, (color1.g + color2.g)/2, (color1.b + color2.b)/2);
}

bool LED_Simulation::ledRing_colorWipeShortestWay(int ringIndexStart, int ringIndexEnd){
    int distance = ringIndexStart - ringIndexEnd;
    if(distance <= -(NUMBER_OF_LEDS_RING/2)){
        return ledRing->colorWipe(TIME_OF_COLOR_WIPE, true, ringIndexStart, ringIndexEnd);
    }else if(distance < 0){
        return ledRing->colorWipe(TIME_OF_COLOR_WIPE, false, ringIndexStart, ringIndexEnd);
    }else if(distance < NUMBER_OF_LEDS_RING/2){
        return ledRing->colorWipe(TIME_OF_COLOR_WIPE, true, ringIndexStart, ringIndexEnd);
    }else{
        return ledRing->colorWipe(TIME_OF_COLOR_WIPE, false, ringIndexStart, ringIndexEnd);
    }
}

void LED_Simulation::simulation3(COLOR color){
    //do only at beginning of simulation
    if(simulationFinished){
        simulationFinished = false;
    }

    switch(simulationStage){
        case 0:
            allLEDStripsFinished = true;
            for(int i = 0; i < NUMBER_OF_ALL_LED_STRIPS - 1; i++){
                if(!ledStripsFinished[i]){
                    ledStrips[i]->setColor(color);
                    ledStripsFinished[i] = ledStrips[i]->colorWipeOneByOne(TIME_OF_COLOR_WIPE, true, ledStrips[i]->getNumberOfLEDs() - 1);
                    allLEDStripsFinished = (allLEDStripsFinished && ledStripsFinished[i]);
                }
            }
            nextSimulationStage(allLEDStripsFinished);
            break;
        case 1:
            clearAllStrips();
            nextSimulationStage(pause(TIME_OF_COLOR_WIPE));
            break;
        case 2:
            ledRing->setColor(color);
            allLEDStripsFinished = ledRing->plainColor(TIME_OF_COLOR_WIPE, 2);
            nextSimulationStage(allLEDStripsFinished);
            break;
        case 3:
            allLEDStripsFinished = true;
            for(int i = 0; i < NUMBER_OF_ALL_LED_STRIPS - 1; i++){
                if(!ledStripsFinished[i]){
                    ledStrips[i]->setColor(color);
                    ledStripsFinished[i] = ledStrips[i]->colorWipeOneByOne(TIME_OF_COLOR_WIPE);
                    allLEDStripsFinished = (allLEDStripsFinished && ledStripsFinished[i]);
                }
            }
            nextSimulationStage(allLEDStripsFinished);
            break;
        case 4:
            clearAllStrips();
            nextSimulationStage(pause(TIME_OF_COLOR_WIPE));
            break;
        case 5:
            counterSimulation3++;
            nextSimulation(3);
            break;
        default:
            clearAllStrips();
            break;
    }
}
