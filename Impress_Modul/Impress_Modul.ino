#include "Force_Sensor.h"
#include "OLED_Display.h"
#include "MQTT.h"

//------------changeable-variables---------------------------------
//Force Sensor variables
#define FORCE_SENSOR1_PIN 34
#define FORCE_SENSOR2_PIN 35

const float alpha = 0.05;
const int frequencyOfDisplayWeight = 1;
const int frequencyOfForceSensorReadings = 200;

//Connection variables
const char* ssid = "FIWARE";
const char* password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.20.56";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "LegoDemonstrator1234";
const char* mqtt_client_id = "ESP32ImpressModul";

const char* mqtt_LED_cmd_topic = "/mqtt-key/LED002/cmd";
const char* mqtt_LED_cmdexe_topic = "/mqtt-key/LED002/cmdexe";
const char* mqtt_weight_topic = "/mqtt-key/weight001/attrs";


//-----------non-changeable-variables-----------------------------------
//Force Sensor variables
int movingAvarageTotal = 0;
int weightClass = 0;
Force_Sensor forceSensor(FORCE_SENSOR1_PIN, FORCE_SENSOR2_PIN, alpha);

//OLED Display variables
OLED_Display oledDisplay;

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt;

//JSON
DynamicJsonDocument jsonDoc(1024);

//-----------------------setup-----------------------------------------
void setup() {
    Serial.begin(115200);
    oledDisplay.setup();
    mqtt.setup();
}

//-----------------------loop------------------------------------------
void loop() {
    //Reconnect to MQTT if disconnected
    mqtt.loop();

    //read the force sensor a number of times 
    int numberOfForceSensorReadings = frequencyOfForceSensorReadings/frequencyOfDisplayWeight;
    for(int i = 0; i < numberOfForceSensorReadings; i++){
        forceSensor.read();
        delay(1000/frequencyOfForceSensorReadings);
    }

    //calculate the avarage of the force sensor measurements and calculate the weight
    movingAvarageTotal = forceSensor.calculateAvarage();
    float weight = movingAvarageTotal/100.0;
    weightClass = forceSensor.getWeightClass();

    //send weight over MQTT and display on OLED-Screen
    mqtt.send(mqtt_weight_topic, "weight", weight);
    oledDisplay.display(weight, weightClass,forceSensor.getAvarageOfForceSensor1(),forceSensor.getAvarageOfForceSensor2());
}