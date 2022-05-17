#include "MQTT.h"

//---------------public-----------------------

MQTT::MQTT(const char* wifi_ssid,
 const char* wifi_password, 
 const char* mqtt_server, 
 const char* mqtt_username, 
 const char* mqtt_password, 
 const char* mqtt_client_id)
{
    this->wifi_ssid = wifi_ssid;
    this->wifi_password = wifi_password;
    this->mqtt_server = mqtt_server;
    this->mqtt_username = mqtt_username;
    this->mqtt_password = mqtt_password;
    this->mqtt_client_id = mqtt_client_id;
}

void MQTT::setup() {
    initWiFi();
    int i = 0;
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
        if(i++ > 20) break;
    }
    if(WiFi.status() == WL_CONNECTED){
        Serial.println("");
        Serial.println("WiFi connected!");
    }else{
        Serial.println("");
        Serial.println("WiFi is not connected!");
    }
    initMQTT();
}

void MQTT::loop() {
    if(!client.connected()) mqtt_reconnect();
    client.loop();
}

void MQTT::send(const char* topic, const char* sensor, float measurement) {
    if(client.connected()){
        String responseMessage = ""; 
        jsonDoc.clear();

        jsonDoc[sensor] = measurement;
        serializeJson(jsonDoc, responseMessage);

        client.publish(topic, responseMessage.c_str());
    }
}

void MQTT::send(const char* topic, const char* attribute, const char* string) {
    if(client.connected()){
        String responseMessage = ""; 
        jsonDoc.clear();

        jsonDoc[attribute] = string;
        serializeJson(jsonDoc, responseMessage);

        client.publish(topic, responseMessage.c_str());
    }
}

//-------------private-------------------------

//WiFi
void MQTT::initWiFi() {
    // delete old config
    WiFi.disconnect(true);

    delay(100);

    WiFi.setAutoReconnect(true);

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    
    Serial.println("");
    Serial.print("Wait for WiFi...");
}

//MQTT
void MQTT::initMQTT() {
    client.setServer(mqtt_server,1883);
    client.setCallback(mqtt_callback);
    Serial.println("Initialized MQTT!");
}

void MQTT::mqtt_callback(char* topic, byte* message, unsigned int length) {
    String messageTemp = "";
    
    Serial.print("Message arrived on topic: ");
    Serial.println(String(topic));
    Serial.print("Message: ");

    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);

    jsonDoc.clear();
    auto error = deserializeJson(jsonDoc, messageTemp);
    if (error) {
        Serial.println("deserializeJson() failed with code ");
        Serial.println(error.c_str());
        return;
    }

    mqttCallback(String(topic));
}

void MQTT::mqtt_reconnect() {
    long currentTime = millis();
    if(!client.connected() && (currentTime - lastTime > MQTT_RECONNECT_TIME * 1000)){
        lastTime = currentTime;

        Serial.println("Connecting MQTT ...");
        if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
            Serial.println("MQTT connected!");
            // Subscribe
            for(auto topic = topics.begin(); topic != topics.end(); ++topic){
                client.subscribe(*topic);
            }
            delay(1000);
        } else {
            delay(100);
            Serial.println(String("failed, rc=" + String(client.state())));
            delay(100);
            String string_retry = "try again in " + String(MQTT_RECONNECT_TIME) + " sec";
            Serial.println(string_retry);
            delay(1000);
        }
    }
}

void MQTT::subscribe(const char* topic){
    topics.push_back(topic);
}