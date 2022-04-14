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
const int frequencyOfDisplayWeight = 1; //Hz
const int frequencyOfForceSensorReadings = 200; //Hz

//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "ESP32ImpressModul";

const char* mqtt_LED_cmd_topic = "/idFZy8D9KzFko7db/LED002/cmd";
const char* mqtt_LED_cmdexe_topic = "/idFZy8D9KzFko7db/LED002/cmdexe";
const char* mqtt_weight_topic = "/idFZy8D9KzFko7db/weight001/attrs";


//-----------non-changeable-variables-----------------------------------
//Force Sensor variables
long lastTime = 0;
int movingAvarageTotal = 0;
int weightClass = 0;
int price = 0;
int countForceSensorReadings = 0;
const int numberOfForceSensorReadings = frequencyOfForceSensorReadings/frequencyOfDisplayWeight;
Force_Sensor forceSensor(FORCE_SENSOR1_PIN, FORCE_SENSOR2_PIN, alpha);

//OLED Display variables
OLED_Display oledDisplay;

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

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
    mqtt.subscribe(mqtt_LED_cmd_topic);

    ledStrip1.setup();
    ledStrip2.setup();
    ledPWM.setup();

    ledStrip1.setColor(255,255,255);
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
        price = forceSensor.getPrice();


        //Reconnect to MQTT if disconnected
        mqtt.loop();

        //send weight over MQTT and display on OLED-Screen
        mqtt.send(mqtt_weight_topic, "weight", weight);
        mqtt.send(mqtt_weight_topic, "weightClass", weightClass);
        mqtt.send(mqtt_weight_topic, "price", price);
        oledDisplay.display(weight, weightClass, price, forceSensor.getAvarageOfForceSensor1(),forceSensor.getAvarageOfForceSensor2());
    }

    //update LEDs
    ledStrip1.loop();
    //ledStrip2.loop();
    ledStrip2.weldingWork(1000);
    ledPWM.loop();
}

//MQTT callback
void mqttCallback(String topic){
    String responseMessage = ""; 

    if(topic == mqtt_LED_cmd_topic){
        String LED_on = jsonDoc["on"];
        String LED_off = jsonDoc["off"];
        String LED_mode = jsonDoc["mode"];

        Serial.print("LED_on: ");
        Serial.println(LED_on);
        Serial.print("LED_off: ");
        Serial.println(LED_off);
        Serial.print("LED_mode: ");
        Serial.println(LED_mode);

        if(LED_on != "null"){
            if(LED_on == ""){
                jsonDoc["on"] = "LED is on!";
                ledStrip1.setColor(255,255,255);
            }
            else{
                int commaIndex = LED_on.indexOf(',');
                int secondCommaIndex = LED_on.indexOf(',', commaIndex + 1);

                String str_r = LED_on.substring(0, commaIndex);
                String str_g = LED_on.substring(commaIndex + 1, secondCommaIndex);
                String str_b = LED_on.substring(secondCommaIndex + 1);

                int value_r = str_r.toInt();
                int value_g = str_g.toInt();
                int value_b = str_b.toInt();

                if(value_r >= 0 && value_g >= 0 && value_b >= 0 && value_r <= 255 && value_g <= 255 && value_b <= 255){
                    jsonDoc["on"] = "LED is on with Color" + str_r + "," + str_g + "," + str_b + " !";
                    ledStrip1.setColor(value_r,value_g,value_b);
                }else{
                    jsonDoc["on"] = "Message body is wrong !";
                }
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
        else if(LED_off != "null"){
            if(LED_off == ""){
                jsonDoc["off"] = "LED is off!";
                ledStrip1.setColor(0,0,0);
            }
            else{
                jsonDoc["off"] = "Message body should be empty !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
        else if(LED_mode != "null"){
            int LED_mode_int = LED_mode.toInt();
            if(LED_mode_int >= 0 && LED_mode_int <= NUMBER_OF_LED_MODES){
                jsonDoc["mode"] = "LED is in mode " + LED_mode;
                ledStrip1.setMode(LED_mode_int);
            }
            else{
                jsonDoc["mode"] = "Message body is wrong !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
    }
}