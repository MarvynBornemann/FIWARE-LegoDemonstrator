#include "MQTT.h"
#include "OLED_Display.h"
#include "LED_Bar.h"
#include <Ultrasonic.h>

//------------changeable-variables---------------------------------
//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "Mobility-ESP8266ElectricVehicleChargingStation";

const char* mqtt_electricVehicleChargingStation001_topic = "/idFZy8D9KzFko7db/electricVehicleChargingStation001/attrs";
const char* mqtt_electricVehicleChargingStation002_topic = "/idFZy8D9KzFko7db/electricVehicleChargingStation002/attrs";

//OLED Display variables
const int OLED_DISPLAY_SCL_PIN = 5;
const int OLED_DISPLAY_SCA_PIN = 4;

//Ultrasonic sensors
const int ULTRASONIC1_PIN = 12;
const int ULTRASONIC2_PIN = 14;

//LED Bar Graph
const int DI_LED_BAR1_PIN = 3;
const int DCKI_LED_BAR1_PIN = 1;
const int DI_LED_BAR2_PIN = 13;
const int DCKI_LED_BAR2_PIN = 2;

//sensor reading variables
const int frequencyOfSensorReadings = 1; //Hz
const int distanceThreshold = 6; //cm

//-----------non-changeable-variables-----------------------------------
//Ultrasonic sensors
Ultrasonic ultrasonic1(ULTRASONIC1_PIN);
Ultrasonic ultrasonic2(ULTRASONIC2_PIN);

//LED_Bar
LED_Bar ledBar1(DCKI_LED_BAR1_PIN, DI_LED_BAR1_PIN);
LED_Bar ledBar2(DCKI_LED_BAR2_PIN, DI_LED_BAR2_PIN);

//OLED Display variables
OLED_Display oledDisplay(OLED_DISPLAY_SCA_PIN, OLED_DISPLAY_SCL_PIN);

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

//JSON
DynamicJsonDocument jsonDoc(1024);

//variables
unsigned long lastTime = 0;

int countSensorReadings = 0;

unsigned int distanceUltrasonic1 = 0;
unsigned int distanceUltrasonic2 = 0;

bool parkingSpot1_available = true;
bool parkingSpot2_available = true;

int levelOfCharging1 = 0;
int levelOfCharging2 = 0;
int levelOfCharging1_old = 0;
int levelOfCharging2_old = 0;

long timePerLevel1 = TIME_PER_LEVEL_MIN;
long timePerLevel2 = TIME_PER_LEVEL_MIN;
int counterTimePerLevel1 = 0;
int counterTimePerLevel2 = 0;

int remainingTimeForFullCharge1 = 0;
int remainingTimeForFullCharge2 = 0;


void setup() {
    //Wio node -> make port available
    pinMode(15, OUTPUT);
    digitalWrite(15, 1);  
    
    //Serial.begin(115200);
    oledDisplay.setup();

    mqtt.setup();

    ledBar1.setup();
    ledBar2.setup();
}

void loop() {
    mqtt.loop();

    //loop for ledBars
    if(!parkingSpot1_available){
        levelOfCharging1 = ledBar1.charging(timePerLevel1);
    }
    if(!parkingSpot2_available){
        levelOfCharging2 = ledBar2.charging(timePerLevel2);
    }

    unsigned long currentTime = millis();
    unsigned long diffTime = currentTime - lastTime;
    if(diffTime > 1000/frequencyOfSensorReadings){
        lastTime = currentTime;

        countSensorReadings++;

        //read sensors
        distanceUltrasonic1 = ultrasonic1.read();
        distanceUltrasonic2 = ultrasonic2.read();

        //charging station is occupied if distance of ultrasonic sensor is below a threshold
        if(distanceUltrasonic1 > distanceThreshold){
            parkingSpot1_available = true;
            ledBar1.reset();
            levelOfCharging1 = 0;
            levelOfCharging1_old = 0;
            remainingTimeForFullCharge1 = 0;
        }else{
            //get new random charging time if charging station gets occupied (parkingSpot1_available changes from true to false)
            if(parkingSpot1_available){
                timePerLevel1 = random(TIME_PER_LEVEL_MIN, TIME_PER_LEVEL_MAX);
                counterTimePerLevel1 = 0;
            }
            parkingSpot1_available = false;
        }
        if(distanceUltrasonic2 > distanceThreshold){
            parkingSpot2_available = true;
            ledBar2.reset();
            levelOfCharging2 = 0;
            levelOfCharging2_old = 0;
            remainingTimeForFullCharge2 = 0;
        }else{
            //get new random charging time if charging station gets occupied (parkingSpot2_available changes from true to false)
            if(parkingSpot2_available){
                timePerLevel2 = random(TIME_PER_LEVEL_MIN, TIME_PER_LEVEL_MAX);
                counterTimePerLevel2 = 0;
            }
            parkingSpot2_available = false;
        }

        //calculate remaining time for full charge
        if(!parkingSpot1_available){
            if(levelOfCharging1 != NUMBER_OF_LEVELS){
                if(levelOfCharging1 != levelOfCharging1_old){
                    levelOfCharging1_old = levelOfCharging1;
                    counterTimePerLevel1 = 0;
                }else counterTimePerLevel1++;
                float timePerLevel1_Sec = timePerLevel1 / 1000.0;
                remainingTimeForFullCharge1 = (NUMBER_OF_LEVELS - levelOfCharging1) * timePerLevel1_Sec - counterTimePerLevel1;
            }else remainingTimeForFullCharge1 = 0;
        }
        
        if(!parkingSpot2_available){
            if(levelOfCharging2 != NUMBER_OF_LEVELS){
                if(levelOfCharging2 != levelOfCharging2_old){
                    levelOfCharging2_old = levelOfCharging2;
                    counterTimePerLevel2 = 0;
                }else counterTimePerLevel2++;
                float timePerLevel2_Sec = timePerLevel2 / 1000.0;
                remainingTimeForFullCharge2 = (NUMBER_OF_LEVELS - levelOfCharging2) * timePerLevel2_Sec - counterTimePerLevel2;
            }else remainingTimeForFullCharge2 = 0;
        }

        //calculate levelOfCharging in percent
        int levelOfCharging1_percent = levelOfCharging1 * 100 / NUMBER_OF_LEVELS;
        int levelOfCharging2_percent = levelOfCharging2 * 100 / NUMBER_OF_LEVELS;

        //mqtt
        mqtt.send(mqtt_electricVehicleChargingStation001_topic, "distanceUltrasonic", distanceUltrasonic1);
        if(parkingSpot1_available) mqtt.send(mqtt_electricVehicleChargingStation001_topic, "status", "free");
        else mqtt.send(mqtt_electricVehicleChargingStation001_topic, "status", "occupied");
        mqtt.send(mqtt_electricVehicleChargingStation001_topic, "levelOfCharging", levelOfCharging1_percent);
        mqtt.send(mqtt_electricVehicleChargingStation001_topic, "remainingTimeForFullCharge", remainingTimeForFullCharge1);

        mqtt.send(mqtt_electricVehicleChargingStation002_topic, "distanceUltrasonic", distanceUltrasonic2);
        if(parkingSpot2_available) mqtt.send(mqtt_electricVehicleChargingStation002_topic, "status", "free");
        else mqtt.send(mqtt_electricVehicleChargingStation002_topic, "status", "occupied");
        mqtt.send(mqtt_electricVehicleChargingStation002_topic, "levelOfCharging", levelOfCharging2_percent);
        mqtt.send(mqtt_electricVehicleChargingStation002_topic, "remainingTimeForFullCharge", remainingTimeForFullCharge2);

        //show on OLED-Display
        if(countSensorReadings < 4){
            int numberOfFreeParkingSpots = parkingSpot1_available + parkingSpot2_available;
            oledDisplay.displayElectricChargingSign(numberOfFreeParkingSpots);
        }else if(countSensorReadings > 10){
            countSensorReadings = 0;
        }else{
            oledDisplay.displayElectricChargingProgress(parkingSpot1_available, parkingSpot2_available, remainingTimeForFullCharge1, remainingTimeForFullCharge2);
        }
    }    
}

void mqttCallback(String topic){
    Serial.println(topic);
}