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
const char* mqtt_client_id = "Mobility-ESP8266ParkingSpot";

const char* mqtt_parkingSpot001_topic = "/idFZy8D9KzFko7db/ParkingSpot:Mobility_Hub:1/attrs";
const char* mqtt_parkingSpot002_topic = "/idFZy8D9KzFko7db/ParkingSpot:Mobility_Hub:2/attrs";
const char* mqtt_parkingSpot003_topic = "/idFZy8D9KzFko7db/ParkingSpot:Mobility_Hub:3/attrs";
const char* mqtt_parkingSpot004_topic = "/idFZy8D9KzFko7db/ParkingSpot:Mobility_Hub:4/attrs";

//OLED Display variables
const int OLED_DISPLAY_SCL_PIN = 5;
const int OLED_DISPLAY_SCA_PIN = 4;

//Ultrasonic sensors
const int ULTRASONIC1_PIN = 14;
const int ULTRASONIC2_PIN = 12;
const int ULTRASONIC3_PIN = 13;
const int ULTRASONIC4_PIN = 3;


//sensor reading variables
const int frequencyOfSensorReadings = 1; //Hz
const int distanceThreshold = 10; //cm

//-----------non-changeable-variables-----------------------------------
//Ultrasonic sensors
Ultrasonic ultrasonic1(ULTRASONIC1_PIN);
Ultrasonic ultrasonic2(ULTRASONIC2_PIN);
Ultrasonic ultrasonic3(ULTRASONIC3_PIN);
Ultrasonic ultrasonic4(ULTRASONIC4_PIN);

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
unsigned int distanceUltrasonic4 = 0;

bool parkingSpot1_available = false;
bool parkingSpot2_available = false;
bool parkingSpot3_available = false;
bool parkingSpot4_available = false;


void setup() { 
    //Wio node -> make port available
    pinMode(15, OUTPUT);
    digitalWrite(15, 1);  

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
        distanceUltrasonic4 = ultrasonic4.read();

        if(distanceUltrasonic1 > distanceThreshold) parkingSpot1_available = true;
        else parkingSpot1_available = false;
        if(distanceUltrasonic2 > distanceThreshold) parkingSpot2_available = true;
        else parkingSpot2_available = false;
        if(distanceUltrasonic3 > distanceThreshold) parkingSpot3_available = true;
        else parkingSpot3_available = false;
        if(distanceUltrasonic4 > distanceThreshold) parkingSpot4_available = true;
        else parkingSpot4_available = false;

        //mqtt
        if(parkingSpot1_available) mqtt.send(mqtt_parkingSpot001_topic, "status", "free");
        else mqtt.send(mqtt_parkingSpot001_topic, "status", "occupied");
        if(parkingSpot2_available) mqtt.send(mqtt_parkingSpot002_topic, "status", "free");
        else mqtt.send(mqtt_parkingSpot002_topic, "status", "occupied");
        if(parkingSpot3_available) mqtt.send(mqtt_parkingSpot003_topic, "status", "free");
        else mqtt.send(mqtt_parkingSpot003_topic, "status", "occupied");
        if(parkingSpot4_available) mqtt.send(mqtt_parkingSpot004_topic, "status", "free");
        else mqtt.send(mqtt_parkingSpot004_topic, "status", "occupied");

        if(countSensorReadings < 4){
            int numberOfFreeParkingSpots = parkingSpot1_available + parkingSpot2_available + parkingSpot3_available + parkingSpot4_available;
            oledDisplay.displayParkingSign(numberOfFreeParkingSpots);
        }else if(countSensorReadings > 10){
            countSensorReadings = 0;
        }else{
            oledDisplay.displayParkingSpots(parkingSpot1_available, parkingSpot2_available, parkingSpot3_available, parkingSpot4_available);
        }
    }    
}

void mqttCallback(String topic){
    Serial.println(topic);
}