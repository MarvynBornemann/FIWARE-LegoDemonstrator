#pragma once

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>

#define MQTT_RECONNECT_TIME 120 //in seconds

//JSON
extern DynamicJsonDocument jsonDoc;

//MQTT
extern PubSubClient client;
extern void mqttCallback(String topic);


class MQTT
{
    private:
        //WiFi
        const char* wifi_ssid;
        const char* wifi_password;
        //MQTT server
        const char* mqtt_server;
        const char* mqtt_username;
        const char* mqtt_password;
        const char* mqtt_client_id;

        //MQTT topics
        std::vector<const char*> topics;

        //WiFi
        void initWiFi();

        //MQTT
        void initMQTT();
        static void mqtt_callback(char* topic, byte* message, unsigned int length);
        void mqtt_reconnect();

        long lastTime = 0;

    public:
        MQTT(const char* wifi_ssid, 
            const char* wifi_password, 
            const char* mqtt_server, 
            const char* mqtt_username, 
            const char* mqtt_password, 
            const char* mqtt_client_id
        );
        
        void subscribe(const char* topic);

        void setup();
        void loop();
        void send(const char* topic, const char* sensor, float measurement);
        void send(const char* topic, const char* attribute, const char* string);
};