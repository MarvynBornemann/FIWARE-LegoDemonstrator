#pragma once

#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>

#include "OLED_Display.h"

#define MQTT_RECONNECT_TIME 120 //in seconds

//JSON
extern DynamicJsonDocument jsonDoc;

//MQTT
extern PubSubClient client;
extern void mqttCallback(String topic);

//OLED Display
extern OLED_Display oledDisplay;


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
};