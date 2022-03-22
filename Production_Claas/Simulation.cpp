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
    rgbLED1->setup();
    rgbLED2->setup();
    attachServos();
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

void Simulation::attachServos(){
    servoScanner->attach();
    servoRobot1->attach();
    servoRobot2->attach();
}

void Simulation::detachServos(){
    servoScanner->detach();
    servoRobot1->detach();
    servoRobot2->detach();
}

void Simulation::loop(){
    switch(simulationIndex){
        case 0:
            turnEverythingOff();
            break;
        case 1:
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
            nextSimulationStage(pause(100));
            break;
        case 1:
            ledStripScanner->setColor(COLOR(255,0,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, false, 0 , ledStripScanner->getNumberOfLEDs()/2 - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 2:
            ledStripScanner->setColor(COLOR(255,0,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs()/2 - 1, 0);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 3:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 4:
            servoScanner->rotate(15, false);
            nextSimulationStage(pause(100));
            break;
        case 5:
            ledStripScanner->setColor(COLOR(255,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, false, 0 , ledStripScanner->getNumberOfLEDs()/2 - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 6:
            ledStripScanner->setColor(COLOR(255,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs()/2 - 1, 0);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 7:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 8:
            servoScanner->rotate(10);
            nextSimulationStage(pause(100));
            break;
        case 9:
            ledStripScanner->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, false, 0 , ledStripScanner->getNumberOfLEDs()/2 - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 10:
            servoScanner->rotate(10, false);
            nextSimulationStage(pause(100));
            break;
        case 11:
            ledStripScanner->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs()/2 - 1, 0);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 12:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 13:
            ledStripDatastream->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripDatastream->dataWipe(TIME_OF_DATASTREAM, datastream, datastreamLength, 3);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 14:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 15:
            //attachServos();
            nextSimulationStage(pause(100));
            break;
        case 16:
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 180, 5);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO, 180, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 17:
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 90, 5);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO, 90, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 18:
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 180, 5);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO, 180, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 19:
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 0, 5);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO, 0, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        // case 20:
        //     //detachServos();
        //     nextSimulationStage(pause(100));
        //     break;
        case 20:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 21:
            rgbLED2->setColor(COLOR(255,0,0));
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_DATASTREAM, 15, 5);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 22:
            stagePartFinished[0] = rgbLED1->weldingWork(200);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 23:
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
            nextSimulation(2);
            break;
        default:
            turnEverythingOff();
            break;
    }
}