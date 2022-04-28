#include "MQTT.h"
#include "OLED_Display.h"
#include <Ultrasonic.h>

//------------changeable-variables---------------------------------
//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "Harbour-ESP8266BoatBerths";

const char* mqtt_boatBerth001_topic = "/idFZy8D9KzFko7db/ParkingSpot:Harbour:1/attrs";
const char* mqtt_boatBerth002_topic = "/idFZy8D9KzFko7db/ParkingSpot:Harbour:2/attrs";
const char* mqtt_boatBerth003_topic = "/idFZy8D9KzFko7db/ParkingSpot:Harbour:3/attrs";

//OLED Display variables
const int OLED_DISPLAY_SCL_PIN = 5;
const int OLED_DISPLAY_SCA_PIN = 4;

//Ultrasonic sensors
const int ULTRASONIC1_TRIGGER_PIN = 0;
const int ULTRASONIC1_ECHO_PIN = 2;
const int ULTRASONIC2_TRIGGER_PIN = 14;
const int ULTRASONIC2_ECHO_PIN = 12;
const int ULTRASONIC3_TRIGGER_PIN = 13;
const int ULTRASONIC3_ECHO_PIN = 15;


//sensor reading variables
const int frequencyOfSensorReadings = 1; //Hz
const int distanceThreshold = 10; //cm

//-----------non-changeable-variables-----------------------------------
//Ultrasonic sensors
Ultrasonic ultrasonic1(ULTRASONIC1_TRIGGER_PIN, ULTRASONIC1_ECHO_PIN);
Ultrasonic ultrasonic2(ULTRASONIC2_TRIGGER_PIN, ULTRASONIC2_ECHO_PIN);
Ultrasonic ultrasonic3(ULTRASONIC3_TRIGGER_PIN, ULTRASONIC3_ECHO_PIN);

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
unsigned int distanceUltrasonic3 = 0;

bool boatBerth1_available = false;
bool boatBerth2_available = false;
bool boatBerth3_available = false;


void setup() { 
    //Serial.begin(115200);
    oledDisplay.setup();

    mqtt.setup();
}

void loop() {
    mqtt.loop();

    unsigned long currentTime = millis();
    unsigned long diffTime = currentTime - lastTime;

    if(diffTime > 1000/frequencyOfSensorReadings){
        lastTime = currentTime;

        countSensorReadings++;

        //read sensors
        distanceUltrasonic1 = ultrasonic1.read();
        distanceUltrasonic2 = ultrasonic2.read();
        distanceUltrasonic3 = ultrasonic3.read();

        if(distanceUltrasonic1 > distanceThreshold) boatBerth1_available = true;
        else boatBerth1_available = false;
        if(distanceUltrasonic2 > distanceThreshold) boatBerth2_available = true;
        else boatBerth2_available = false;
        if(distanceUltrasonic3 > distanceThreshold) boatBerth3_available = true;
        else boatBerth3_available = false;

        //mqtt
        if(boatBerth1_available) mqtt.send(mqtt_boatBerth001_topic, "status", "free");
        else mqtt.send(mqtt_boatBerth001_topic, "status", "occupied");
        if(boatBerth2_available) mqtt.send(mqtt_boatBerth002_topic, "status", "free");
        else mqtt.send(mqtt_boatBerth002_topic, "status", "occupied");
        if(boatBerth3_available) mqtt.send(mqtt_boatBerth003_topic, "status", "free");
        else mqtt.send(mqtt_boatBerth003_topic, "status", "occupied");


        if(countSensorReadings < 4){
            int numberOfFreeBoatBerths = boatBerth1_available + boatBerth2_available + boatBerth3_available;
            oledDisplay.displayParkingSign(numberOfFreeBoatBerths);
        }else if(countSensorReadings > 10){
            countSensorReadings = 0;
        }else{
            oledDisplay.displayBoatBerths(boatBerth1_available, boatBerth2_available, boatBerth3_available);
        }
    }    
}

void mqttCallback(String topic){
    Serial.println(topic);
}