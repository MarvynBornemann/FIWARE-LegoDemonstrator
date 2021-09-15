#pragma once

#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "OLED_Display.h"

extern const char* ssid;
extern const char* password;

extern const char* mqtt_server;
extern const char* mqtt_username;
extern const char* mqtt_password;
extern const char* mqtt_client_id;

extern const char* mqtt_LED_cmd_topic;
extern const char* mqtt_LED_cmdexe_topic;
extern const char* mqtt_weight_topic;

//JSON
extern DynamicJsonDocument jsonDoc;

//MQTT
extern PubSubClient client;

//OLED Display
extern OLED_Display oledDisplay;


class MQTT
{
    private:
        //WiFi
        void initWiFi();
        static void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
        static void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
        static void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

        //MQTT
        void initMQTT();
        static void mqtt_callback(char* topic, byte* message, unsigned int length);
        void mqtt_reconnect();

    public:
        MQTT();
        ~MQTT();

        void setup();
        void loop();
        void send(const char* topic, const char* sensor, float measurement);
};