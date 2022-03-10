#include "Simulation.h"

Simulation::Simulation(){
    ledStripScanner = new LED_Strip(NUMBER_OF_LEDS_STRIP1, LED_STRIP1_PIN);
    ledStripDatastream = new LED_Strip(NUMBER_OF_LEDS_STRIP2, LED_STRIP2_PIN);

    servoScanner = new MyServo(SERVO1_PIN, true);
    servoRobot1 = new MyServo(SERVO2_PIN, false);
    servoRobot2 = new MyServo(SERVO3_PIN, false);

    rgbLED1 = new RGB_LED(RED_LED1_PIN, GREEN_LED1_PIN, BLUE_LED1_PIN);
    rgbLED2 = new RGB_LED(RED_LED2_PIN, GREEN_LED2_PIN, BLUE_LED2_PIN);

    simulationStage = 0;
    simulationIndex = 1;
    simulationFinished = true;

    for(int i = 0; i < NUMBER_OF_STAGE_PARTS_MAXIMUM; i++){
        stagePartFinished[i] = false;
    }
}

void Simulation::nextSimulationStage(bool _stageFinished){
    if(_stageFinished){
        simulationStage++;
        for(int i = 0; i < NUMBER_OF_STAGE_PARTS_MAXIMUM; i++){
            stagePartFinished[i] = false;
        }
    }
}

void Simulation::nextSimulation(int simulationIndex){
    this->simulationIndex = simulationIndex;
    simulationStage = 0;
    simulationFinished = true;
    turnEverythingOff();
}

bool Simulation::pause(long milliseconds){
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

void Simulation::setup(){
    ledStripScanner->setup();
    ledStripDatastream->setup();
    servoScanner->setup();
    servoRobot1->setup();
    servoRobot2->setup();
    rgbLED1->setup();
    rgbLED2->setup();
}

void Simulation::turnEverythingOff(){
    ledStripScanner->clear();
    ledStripDatastream->clear();
    servoScanner->stop();
    servoRobot1->stop();
    servoRobot2->stop();
    rgbLED1->clear();
    rgbLED2->clear();
}

void Simulation::loop(){
    switch(simulationIndex){
        case 0:
            turnEverythingOff();
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
        default:
            turnEverythingOff();
            break;
    }
}

void Simulation::simulation1(){
    //do only at beginning of simulation
    if(simulationFinished){
        simulationFinished = false;
    }
    switch(simulationStage){
        case 0:
            servoScanner->rotate(15);
            ledStripScanner->setColor(COLOR(255,0,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne(TIME_OF_COLOR_WIPE);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 1:
            ledStripScanner->setColor(COLOR(255,0,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs() - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 2:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 3:
            servoScanner->rotate(15, false);
            ledStripScanner->setColor(COLOR(255,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne(TIME_OF_COLOR_WIPE);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 4:
            ledStripScanner->setColor(COLOR(255,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs() - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 5:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 6:
            servoScanner->rotate(10);
            ledStripScanner->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne(TIME_OF_COLOR_WIPE);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 7:
            servoScanner->rotate(10, false);
            ledStripScanner->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs() - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 8:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 9:
            ledStripDatastream->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripDatastream->dataWipe(TIME_OF_DATASTREAM, datastream, datastreamLength, 3);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 10:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 11:
            stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 90);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 12:
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 0);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO, 180, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 13:
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 90, 2);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = (stagePartFinished && servoRobot2->goToAngle(TIME_OF_SERVO, 0, 10));
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 14:
            rgbLED1->setColor(COLOR(0,255,0));
            stagePartFinished[0] = rgbLED1->plainColor(TIME_OF_PAUSE);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 15:
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 16:
            rgbLED1->setColor(COLOR(255,0,0));
            stagePartFinished[0] = rgbLED1->blinkPlainColor(TIME_OF_DATASTREAM, 15, 5);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 17:
            stagePartFinished[0] = rgbLED1->weldingWork(1000);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 18:
            counterSimulation1++;
            nextSimulation(1);
            break;
        default:
            turnEverythingOff();
            break;
    }
    
}

void Simulation::simulation2(){
    //do only at beginning of simulation
    if(simulationFinished){
        simulationFinished = false;
    }

    switch(simulationStage){
        case 0:
            nextSimulation(1);
            break;
        default:
            turnEverythingOff();
            break;
    }
}