#include <DHT.h>
#include "MQTT.h"

//DHT 11 variables
#define DHTPIN 2     
#define DHTTYPE DHT11

//Windmills
#define WINDMILLS_PIN 1

//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "Addix-ESP8266Temperature001";

const char* mqtt_temperature_topic = "/idFZy8D9KzFko7db/temperature001/attrs";
const char* mqtt_windmills_cmd_topic = "/idFZy8D9KzFko7db/windmills001/cmd";
const char* mqtt_windmills_cmdexe_topic = "/idFZy8D9KzFko7db/windmills001/cmdexe";


//DHT 11
DHT dht(DHTPIN, DHTTYPE);

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

//JSON
DynamicJsonDocument jsonDoc(1024);

void setup() {
    Serial.begin(115200);

    pinMode(WINDMILLS_PIN, OUTPUT);
    digitalWrite(WINDMILLS_PIN, HIGH); // turn windmills off at startup

    mqtt.setup();
    mqtt.subscribe(mqtt_windmills_cmd_topic);

    dht.begin();
}

void loop() {
    mqtt.loop();

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

    Serial.print(" Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.print("C  Heat index: ");
    Serial.print(heatIndex);
    Serial.println("C ");

    mqtt.send(mqtt_temperature_topic, "temperature", temperature);
    delay(500);
    mqtt.send(mqtt_temperature_topic, "humidity", humidity);
    delay(500);
    mqtt.send(mqtt_temperature_topic, "heatIndex", heatIndex);
    
    // Wait a few seconds between measurements.
    delay(1000);
}

void mqttCallback(String topic){
    String responseMessage = ""; 

    if(topic == mqtt_windmills_cmd_topic){
        String windmills_on = jsonDoc["on"];
        String windmills_off = jsonDoc["off"];

        Serial.print("windmills_on: ");
        Serial.println(windmills_on);
        Serial.print("windmills_off: ");
        Serial.println(windmills_off);

        if(windmills_on != "null"){
            if(windmills_on == ""){
                jsonDoc["on"] = "Windmills are on!";
                digitalWrite(WINDMILLS_PIN, LOW);
            }
            else{
                jsonDoc["on"] = "Message body should be empty !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_windmills_cmdexe_topic, responseMessage.c_str());
        }
        else if(windmills_off != "null"){
            if(windmills_off == ""){
                jsonDoc["off"] = "Windmills are off!";
                digitalWrite(WINDMILLS_PIN, HIGH);
            }
            else{
                jsonDoc["off"] = "Message body should be empty !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_windmills_cmdexe_topic, responseMessage.c_str());
        }
    }
}