#include "MQTT.h"
#include "OLED_Display.h"

//Connection variables
const char* wifi_ssid = "FIWARE_fair";
const char* wifi_password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.1.100";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "Lego12Demo34nstr56ator";
const char* mqtt_client_id = "Mobility-ESP8266RideSharePrice";

const char* mqtt_particulateMatter_cmd_topic = "/idFZy8D9KzFko7db/particulateMatter001/cmd";
const char* mqtt_particulateMatter_cmdexe_topic = "/idFZy8D9KzFko7db/particulateMatter001/cmdexe";

//OLED Display variables
OLED_Display oledDisplay;

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

//JSON
DynamicJsonDocument jsonDoc(1024);

//variables
long lastTime = 0;
bool doOnce = false;

bool randomizeParticulateMatter = true;
float particulateMatter = 9.7;
int priceCarSharing = 0;
int priceBikeSharing = 0;

void setup() {    
    Serial.begin(115200);
    oledDisplay.setup();

    mqtt.setup();
    mqtt.subscribe(mqtt_particulateMatter_cmd_topic);
}

void loop() {
    mqtt.loop();

    unsigned long currentTime = millis();
    unsigned long diffTime = currentTime - lastTime;

    if(diffTime > 20000){
        lastTime = currentTime;

        if(randomizeParticulateMatter) particulateMatter = random(200)/10.0;

        if(particulateMatter > 10){
            priceCarSharing = 30;
            priceBikeSharing = 1;
        }else{
            priceCarSharing = 12;
            priceBikeSharing = 3;
        }

        Serial.println("particulateMatter = " + String(particulateMatter));
        oledDisplay.display(particulateMatter, priceCarSharing, priceBikeSharing);

        doOnce = true;
    } else if(diffTime > 15000 && doOnce){
        doOnce = false;
        oledDisplay.drawBitmap();
    }
}

void mqttCallback(String topic){
    Serial.println(topic);
    //ToDo: change particulate matter through context broker
    if(topic == mqtt_particulateMatter_cmd_topic){
        randomizeParticulateMatter = false;
    }
}