// Tranmsitting part for CarParkShare
// Without this we won't be able to transmit the data to MQTT broker
// (0,2) RX,TX (10,11)
// Connect VCC and first alternate pin to 3v3
// Connect ground 

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

SoftwareSerial ESP01(0,2);

#define wifi_ssid "FIWARE"
#define wifi_password "!FIWARE!on!air!"

#define mqtt_server "192.168.20.52"
#define mqtt_port 1883
#define out_topic "Fiware/Dock/ShipParkingLot"

WiFiClient espClient;
PubSubClient client;

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("CarCharge","openhabian","openhabian")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
    }
  }
}

void setup() {
  Serial.begin(9600);
	ESP01.begin(9600);
  setup_wifi();
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  while(ESP01.available()>0){
	  String val = ESP01.readString();
    client.publish(out_topic, String(val).c_str(), true);
	}
  delay(1000);
}