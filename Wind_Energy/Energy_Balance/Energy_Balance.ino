#include "MQTT.h"
#include "OLED_Display.h"

//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "WindEnergy-ESP8266EnergyBalance";

const char* mqtt_energyGenerator_topic = "/idFZy8D9KzFko7db/GreenEnergyGenerator:Wind_Energy:1/attrs";
const char* mqtt_energyConsumer_topic = "/idFZy8D9KzFko7db/EnergyConsumer:Wind_Energy:1/attrs";

//OLED Display variables
const int WIO_NODE_SCL1_PIN = 1;
const int WIO_NODE_SDA1_PIN = 3;
OLED_Display oledDisplay1(WIO_NODE_SDA1_PIN, WIO_NODE_SCL1_PIN);
const int WIO_NODE_SCL2_PIN = 4;
const int WIO_NODE_SDA2_PIN = 5;
const int WIO_NODE_I2C_ADRESS2 = 0x3D;
OLED_Display oledDisplay2(WIO_NODE_SDA2_PIN, WIO_NODE_SCL2_PIN, WIO_NODE_I2C_ADRESS2);

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

void mqttCallback(String topic){
  Serial.println(topic);
}

//JSON
DynamicJsonDocument jsonDoc(1024);

//variables
long lastTime = 0;
bool doOnce = false;

float energyProduced = random(30000,50000)/10.0;
float energyConsumed = random(50000,70000)/10.0;

int counter = 0;

void setup() {
  //Wio node -> make port available
  pinMode(15, OUTPUT);
  digitalWrite(15, 1);  
  
  //Serial.begin(115200);
  oledDisplay1.setup();
  oledDisplay2.setup();

  mqtt.setup();
}

void loop() {
  mqtt.loop();

  unsigned long currentTime = millis();
  unsigned long diffTime = currentTime - lastTime;

  if(diffTime > 20000){
    lastTime = currentTime;

    counter++;
    if(counter > 180){
      counter = 0;
      energyProduced = random(30000,50000)/10.0;
      energyConsumed = random(50000,70000)/10.0;
    }

    int randomSign = random(0,10);
    if(randomSign > 4){
      energyProduced = energyProduced - (random(0,1000)/10.0);
    }else{
      energyProduced = energyProduced + (random(0,1000)/10.0);
    }
    randomSign = random(0,10);
    if(randomSign > 4){
      energyConsumed = energyConsumed - (random(0,1000)/10.0);
    }else{
      energyConsumed = energyConsumed + (random(0,1000)/10.0);
    }

    oledDisplay1.display(energyProduced, "generated:");
    oledDisplay2.display(energyConsumed, "consumed:");

    mqtt.send(mqtt_energyGenerator_topic, "maxEolicPowerGenerated", energyProduced);
    mqtt.send(mqtt_energyConsumer_topic, "p", energyConsumed);

    doOnce = true;

  } else if(diffTime > 15000 && doOnce){
    doOnce = false;
    oledDisplay1.drawBitmap(WindEnergy);
    oledDisplay2.drawBitmap(EnergyConsumption);
  }
}