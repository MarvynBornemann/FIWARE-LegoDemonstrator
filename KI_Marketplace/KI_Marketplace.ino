#include "MQTT.h"
#include "LED_Simulation.h"

//------------changeable-variables---------------------------------
//LED Bar Graph
#define DI_LED_BAR1_PIN 16
#define DCKI_LED_BAR1_PIN 5
#define DI_LED_BAR2_PIN 4
#define DCKI_LED_BAR2_PIN 0


//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "ESP8266KIMarketplace";

const char* mqtt_LED_cmd_topic = "/idFZy8D9KzFko7db/LED003/cmd";
const char* mqtt_LED_cmdexe_topic = "/idFZy8D9KzFko7db/LED003/cmdexe";


//-----------non-changeable-variables-----------------------------------
//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

//JSON
DynamicJsonDocument jsonDoc(1024);

//LED_Simulation
LED_Simulation ledSimulation;

//-----------------------setup-----------------------------------------
void setup() {
    Serial.begin(115200);

    mqtt.setup();
    mqtt.subscribe(mqtt_LED_cmd_topic);

    ledSimulation.setup();
}

//-----------------------loop------------------------------------------
void loop() {
    mqtt.loop();
    //update LED-Simulation
    ledSimulation.loop();
}

//ToDo: anpassen an neues LED setup
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
            }
            else{
                jsonDoc["mode"] = "Message body is wrong !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
    }
}