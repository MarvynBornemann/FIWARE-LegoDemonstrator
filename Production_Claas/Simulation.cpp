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
    detachServos();
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
    //analogWriteFreq(100);

    //setup servos and move them to start position
    attachServos();
    delay(1000);
    detachServos(); //detach servos as often as possible. When servos are attached they interfere with the LED-Strips.
}

void Simulation::turnEverythingOff(){
    ledStripScanner->clear();
    ledStripDatastream->clear();
    rgbLED1->clear();
    rgbLED2->clear();
}

void Simulation::attachServos(){
    //servoScanner->attach();
    servoRobot1->attach();
    servoRobot2->attach();
}

void Simulation::detachServos(){
    //servoScanner->detach();
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
        servoScanner->attach();
    }

    switch(simulationStage){
        case 0:
            servoScanner->attach();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 1:
            servoScanner->rotate(15, random(0,1));
            nextSimulationStage(pause(100));
            break;
        case 2:
            ledStripScanner->setColor(COLOR(255,0,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, false, 0 , ledStripScanner->getNumberOfLEDs()/2 - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 3:
            ledStripScanner->setColor(COLOR(255,100,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs()/2 - 1, 0);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 4:
            ledStripScanner->setColor(COLOR(255,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, false, 0 , ledStripScanner->getNumberOfLEDs()/2 - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 5:
            ledStripScanner->setColor(COLOR(100,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs()/2 - 1, 0);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 6:
            ledStripScanner->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, false, 0 , ledStripScanner->getNumberOfLEDs()/2 - 1);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 7:
            ledStripScanner->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripScanner->colorWipeOneByOne_doubleStrip(TIME_OF_COLOR_WIPE, true, ledStripScanner->getNumberOfLEDs()/2 - 1, 0);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 8:
            servoScanner->stop();
            turnEverythingOff();
            nextSimulationStage(pause(10));
            break;
        case 9:
            servoScanner->detach();
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 10:
            mqtt.send(mqtt_Video_cmd_topic, "play", "");
            nextSimulationStage(true);
        case 11:
            ledStripDatastream->setColor(COLOR(0,255,0));
            stagePartFinished[0] = ledStripDatastream->dataWipe(TIME_OF_DATASTREAM, datastream, datastreamLength, 16);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 12:
            turnEverythingOff();
            nextSimulationStage(pause(100));
            break;
        case 13:
            attachServos();
            nextSimulationStage(pause(TIME_OF_PAUSE));
            break;
        case 14:
            rgbLED2->setColor(COLOR(255,0,0)); // red
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 5);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 15:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 0, 5);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 16:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            stagePartFinished[0] = rgbLED1->weldingWork(50);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 17:
            rgbLED1->clear();
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 2);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 18:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            stagePartFinished[0] = servoRobot2->goToAngle(TIME_OF_SERVO, 0, 5);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 19:
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 2);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 20:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            stagePartFinished[0] = servoRobot2->goToAngle(TIME_OF_SERVO, 40, 5);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 21:
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 2);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 22:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 40, 5);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO * 2, 0, 2);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 23:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            if(!stagePartFinished[0]){
                stagePartFinished[0] = rgbLED1->weldingWork(50);
            }else rgbLED1->clear();
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO * 5, 40, 1);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 24:
            rgbLED1->clear();
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 2);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 25:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO * 10, 0, 1);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = rgbLED1->weldingWork(200);
            }else rgbLED1->clear();
            if(!stagePartFinished[2]){
                stagePartFinished[2] = servoRobot2->goToAngle(TIME_OF_SERVO, 10, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1] && stagePartFinished[2]));
            break;
        case 26:
            rgbLED1->clear();
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 2);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 27:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 40, 5);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO, 30, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 28:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            if(!stagePartFinished[0]){
                stagePartFinished[0] = rgbLED1->weldingWork(50);
            }else rgbLED1->clear();
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO * 10, 0, 2);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 29:
            rgbLED1->clear();
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 2);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 30:
            rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK);
            if(!stagePartFinished[0]){
                stagePartFinished[0] = servoRobot1->goToAngle(TIME_OF_SERVO, 90, 5);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = servoRobot2->goToAngle(TIME_OF_SERVO, 90, 5);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 31:
            stagePartFinished[0] = rgbLED2->blinkPlainColor(TIME_OF_BLINK, STEPS_OF_BLINK, 5);
            nextSimulationStage(stagePartFinished[0]);
            break;
        case 32:
            detachServos();
            nextSimulationStage(pause(100));
            break;
        case 33:
            turnEverythingOff();
            nextSimulationStage(pause(TIME_OF_PAUSE * 5));
            break;
        case 34:
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
            if(!stagePartFinished[0]){
                stagePartFinished[0] = ledStripScanner->rainbow(TIME_OF_RAINBOW, 1000);
            }
            if(!stagePartFinished[1]){
                stagePartFinished[1] = ledStripDatastream->rainbow(TIME_OF_RAINBOW, 1000);
            }
            nextSimulationStage((stagePartFinished[0] && stagePartFinished[1]));
            break;
        case 1:
            nextSimulation(2);
            break;
        default:
            turnEverythingOff();
            break;
    }
}