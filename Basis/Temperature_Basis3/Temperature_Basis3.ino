#include <DHT.h>
#include "MQTT.h"

//DHT 11 variables
#define DHTPIN 2     
#define DHTTYPE DHT11

//Connection variables
const char* wifi_ssid = "FIWARE";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.20.46";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "Basis2-ESP8266Temperature002";

const char* mqtt_temperature_topic = "/idFZy8D9KzFko7db/temperature002/attrs";


//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

void mqttCallback(String topic){
  Serial.println(topic);
}

//JSON
DynamicJsonDocument jsonDoc(1024);

void setup() {
  Serial.begin(115200);

  mqtt.setup();
}

void loop() {
  mqtt.loop();

  float temperature = random(50);
  float humidity = random(50);
  float heatIndex = random(50);

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