#include "MQTT.h"
#include "OLED_Display.h"

//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "TownhallAarhus-ESP8266BusTime";

//const char* mqtt_busTime_topic = "/idFZy8D9KzFko7db/busTime001/attrs";

//OLED Display variables
const int WIO_LINK_SCL_PIN = 4;
const int WIO_LINK_SDA_PIN = 5;
OLED_Display oledDisplay(WIO_LINK_SDA_PIN, WIO_LINK_SCL_PIN);

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

void setup() {
    //Wio Link -> make port available
    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);  
    
    Serial.begin(115200);
    oledDisplay.setup();

    mqtt.setup();
}

void loop() {
    mqtt.loop();

    unsigned long currentTime = millis();
    unsigned long diffTime = currentTime - lastTime;

    if(diffTime > 30000){
      lastTime = currentTime;

      oledDisplay.display();

      //mqtt.send(mqtt_busTime_topic, "busTime", energyProduced);

      doOnce = true;
    } else if(diffTime > 20000){
        oledDisplay.displayLarge();
    } else if(diffTime > 15000 && doOnce){
        doOnce = false;
        oledDisplay.drawBitmap();
    }
}