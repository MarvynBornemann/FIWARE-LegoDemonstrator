#include "MQTT.h"

//---------------public-----------------------

MQTT::MQTT() {
}

MQTT::~MQTT() {
}

void MQTT::setup() {
    initWiFi();
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        oledDisplay.println("...");
    }
    oledDisplay.println("WiFi connected!");
    initMQTT();
}

void MQTT::loop() {
    if(!client.connected()) mqtt_reconnect();
    client.loop();
}

void MQTT::send(const char* topic, const char* sensor, float measurement) {
    String responseMessage = ""; 
    jsonDoc.clear();

    jsonDoc[sensor] = measurement;
    serializeJson(jsonDoc, responseMessage);

    client.publish(topic, responseMessage.c_str());
}

//-------------private-------------------------

//WiFi
void MQTT::initWiFi() {
    // delete old config
    WiFi.disconnect(true);

    delay(100);

    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    oledDisplay.println("");
    oledDisplay.println("Wait for WiFi... ");
}

void MQTT::WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.println("Connected to WiFi successfully!");
}

void MQTT::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.println("WiFi connected");
    Serial.println(String(WiFi.localIP()));
    Serial.println("IP address: ");
}

void MQTT::WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.println("Disconnected from WiFi access point");
    Serial.println("WiFi lost connection. Reason: ");
    Serial.println(String(info.disconnected.reason));
    Serial.println("Trying to Reconnect");
    WiFi.begin(ssid, password);
}

//MQTT
void MQTT::initMQTT() {
    client.setServer(mqtt_server,1883);
    client.setCallback(mqtt_callback);
    Serial.println("Initialized MQTT");
}

void MQTT::mqtt_callback(char* topic, byte* message, unsigned int length) {
    String messageTemp = "";
    String responseMessage = ""; 
    
    Serial.println("Message arrived on topic: ");
    Serial.println(String(topic));
    Serial.println(" . Message: ");

    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);

    jsonDoc.clear();
    auto error = deserializeJson(jsonDoc, messageTemp);
    if (error) {
        Serial.println(String(F("deserializeJson() failed with code ")));
        Serial.println(error.c_str());
        return;
    }

    if(String(topic) == mqtt_LED_cmd_topic){
        String LED_on = jsonDoc["on"];
        String LED_off = jsonDoc["off"];
        Serial.println("LED_on: ");
        Serial.println(LED_on);
        Serial.println("LED_off: ");
        Serial.println(LED_off);

        if(LED_on != "null"){
            if(LED_on == ""){
                jsonDoc["on"] = "LED is on!";
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
            }
            else{
                jsonDoc["off"] = "Message body should be empty !";
            }
            serializeJson(jsonDoc, responseMessage);
            client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
        }
    }

}

void MQTT::mqtt_reconnect() {
    while(!client.connected()){
        oledDisplay.println("Connecting MQTT ...");
        if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
            oledDisplay.println("MQTT connected!");
            // Subscribe
            client.subscribe(mqtt_LED_cmd_topic);
            client.subscribe(mqtt_LED_cmdexe_topic);
            client.subscribe(mqtt_weight_topic);
            delay(1000);
        } else {
            delay(500);
            oledDisplay.println(String("failed, rc=" + String(client.state())));
            delay(500);
            oledDisplay.println("try again in 5 sec");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}