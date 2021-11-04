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
const int WIO_NODE_SCL_PIN = 5;
const int WIO_NODE_SDA_PIN = 4;
OLED_Display oledDisplay(WIO_NODE_SDA_PIN, WIO_NODE_SCL_PIN);

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
MQTT mqtt(wifi_ssid, wifi_password, mqtt_server, mqtt_username, mqtt_password, mqtt_client_id);

//JSON
DynamicJsonDocument jsonDoc(1024);

//variables
unsigned long lastTime = 0;
bool doOnceParticulateMatter = true;
bool doOnceCarSharing = true;

bool randomizeParticulateMatter = true;
float levelParticulateMatter = 9.7;
float priceCarSharing = 0;
float priceBikeSharing = 0;

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

        oledDisplay.displayBikeSharing(priceBikeSharing);

        doOnceParticulateMatter = true;
        doOnceCarSharing = true;
    } else if(diffTime > 15000 && doOnceCarSharing){
        doOnceCarSharing = false;

        oledDisplay.displayCarSharing(priceCarSharing);

    } else if(diffTime > 5000 && doOnceParticulateMatter){
        doOnceParticulateMatter = false;

        if(randomizeParticulateMatter) levelParticulateMatter = random(200)/10.0;

        calculatePrices();

        Serial.println("Level of Particulate Matter = " + String(levelParticulateMatter));
        oledDisplay.displayParticulateMatter(levelParticulateMatter);
    }
}

void calculatePrices(){
    if(levelParticulateMatter > 10){
        priceCarSharing = 27.99;
        priceBikeSharing = 0.50;
    }else{
        priceCarSharing = 11.99;
        priceBikeSharing = 2.50;
    }
}

void mqttCallback(String topic){
    Serial.println(topic);
    String responseMessage = ""; 

    //ToDo: change particulate matter through context broker
    if(topic == mqtt_particulateMatter_cmd_topic){
        String level = jsonDoc["level"];

        Serial.print("Level of Particulate Matter = ");
        Serial.println(level);

        if(level != "null"){
            float levelAsNumber = level.toFloat();
            if(levelAsNumber >= 0 && levelAsNumber <= 100){
                randomizeParticulateMatter = false;
                jsonDoc["level"] = "Set level to " + level;
                levelParticulateMatter = levelAsNumber;
                calculatePrices();
            }else{
                randomizeParticulateMatter = true;
                jsonDoc["level"] = "Got wrong level! Has to be a number between 0 and 100! ";
            }

        }
    }
}