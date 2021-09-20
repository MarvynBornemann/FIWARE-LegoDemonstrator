#include "Force_Sensor.h"
#include "OLED_Display.h"
#include "MQTT.h"
#include "LED_Strip.h"
#include "LED_PWM.h"

//------------changeable-variables---------------------------------
//LED Strips
#define LED_STRIP1_PIN 32
#define NUMBER_OF_LEDS_STRIP1 17

#define LED_STRIP2_PIN 33
#define NUMBER_OF_LEDS_STRIP2 7

//LED PWM
#define LED_PWM_PIN 27 

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
long lastTime = 0;
int movingAvarageTotal = 0;
int weightClass = 0;
int countForceSensorReadings = 0;
const int numberOfForceSensorReadings = frequencyOfForceSensorReadings/frequencyOfDisplayWeight;
Force_Sensor forceSensor(FORCE_SENSOR1_PIN, FORCE_SENSOR2_PIN, alpha);

//OLED Display variables
OLED_Display oledDisplay;

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt;

//JSON
DynamicJsonDocument jsonDoc(1024);

//LED Strips
LED_Strip ledStrip1(NUMBER_OF_LEDS_STRIP1, LED_STRIP1_PIN);
LED_Strip ledStrip2(NUMBER_OF_LEDS_STRIP2, LED_STRIP2_PIN);

//LED PWM
LED_PWM ledPWM(LED_PWM_PIN);

//-----------------------setup-----------------------------------------
void setup() {
    Serial.begin(115200);
    oledDisplay.setup();
    mqtt.setup();
    ledStrip1.setup();
    ledStrip2.setup();
    ledPWM.setup();
}

//-----------------------loop------------------------------------------
void loop() {
    //read the force sensor if its time
    long currentTime = millis();
    if(currentTime - lastTime > 1000/frequencyOfForceSensorReadings){
        lastTime = currentTime;
        forceSensor.read();
        countForceSensorReadings++;
    }

    if(countForceSensorReadings >= numberOfForceSensorReadings){
        countForceSensorReadings = 0;
        
        //calculate the avarage of the force sensor measurements and calculate the weight
        movingAvarageTotal = forceSensor.calculateAvarage();
        float weight = movingAvarageTotal/100.0;
        weightClass = forceSensor.getWeightClass();

        //Reconnect to MQTT if disconnected
        mqtt.loop();

        //send weight over MQTT and display on OLED-Screen
        mqtt.send(mqtt_weight_topic, "weight", weight);
        oledDisplay.display(weight, weightClass,forceSensor.getAvarageOfForceSensor1(),forceSensor.getAvarageOfForceSensor2());
    }

    //update LEDs
    ledStrip1.loop();
    //ledStrip2.loop();
    ledStrip2.weldingWork(1000);
    ledPWM.loop();
}