#include <DHT.h>
#include "MQTT.h"

//DHT 11 variables
#define DHTPIN 2     
#define DHTTYPE DHT11

//Connection variables
const char* ssid = "FIWARE";
const char* password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.20.56";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "LegoDemonstrator1234";
const char* mqtt_client_id = "Addix-ESP8266Temperature001";

const char* mqtt_temperature_topic = "/idFZy8D9KzFko7db/temperature001/attrs";


//DHT 11
DHT dht(DHTPIN, DHTTYPE);

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

void mqttCallback(String topic){
  Serial.println(topic);
}

//JSON
DynamicJsonDocument jsonDoc(1024);

void setup() {
  Serial.begin(115200);

  mqtt.setup();

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