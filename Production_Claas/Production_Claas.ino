#include "MQTT.h"
#include "Simulation.h"

//------------changeable-variables---------------------------------
//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "ESP8266ProductionClaas";

const char* mqtt_LED_cmd_topic = "/idFZy8D9KzFko7db/LED004/cmd";
const char* mqtt_LED_cmdexe_topic = "/idFZy8D9KzFko7db/LED004/cmdexe";
const char* mqtt_Video_cmd_topic = "/idFZy8D9KzFko7db/Video002/cmd";


//-----------non-changeable-variables-----------------------------------
//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

//JSON
DynamicJsonDocument jsonDoc(1024);

//Simulation
Simulation simulation;
int simulationIndex = 1;

//-----------------------setup-----------------------------------------
void setup() {
    //Serial.begin(115200);

    mqtt.setup();
    mqtt.subscribe(mqtt_LED_cmd_topic);

    simulation.setup();
    simulation.nextSimulation(simulationIndex);
}

//-----------------------loop------------------------------------------
void loop() {
    mqtt.loop();
    //update LED-Simulation
    simulation.loop();
}

//ToDo: anpassen an neues LED setup
//MQTT callback
void mqttCallback(String topic){
    String responseMessage = ""; 

    if(topic == mqtt_LED_cmd_topic){
        String LED_on = jsonDoc["on"];
        String LED_off = jsonDoc["off"];
        String LED_simulation = jsonDoc["simulation"];

        Serial.print("LED_on: ");
        Serial.println(LED_on);
        Serial.print("LED_off: ");
        Serial.println(LED_off);
        Serial.print("LED_simulation: ");
        Serial.println(LED_simulation);

        if(LED_on != "null"){
            if(LED_on == ""){
                jsonDoc["on"] = "LED is on!";
                simulation.nextSimulation(simulationIndex);
            }
            else{
                jsonDoc["on"] = "Message body should be empty !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
        else if(LED_off != "null"){
            if(LED_off == ""){
                jsonDoc["off"] = "LED is off!";
                simulation.nextSimulation(0);
            }
            else{
                jsonDoc["off"] = "Message body should be empty !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
        else if(LED_simulation != "null"){
            int LED_simulation_int = LED_simulation.toInt();
            if(LED_simulation_int >= 0 && LED_simulation_int <= NUMBER_OF_SIMULATIONS){
                jsonDoc["simulation"] = "LED runs simulation " + LED_simulation;
                simulationIndex = LED_simulation_int;
                simulation.nextSimulation(simulationIndex);
            }
            else{
                jsonDoc["simulation"] = "Message body is wrong !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
    }
}