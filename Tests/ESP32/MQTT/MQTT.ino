#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "FIWARE";
const char* password = "!FIWARE!on!air!";

const char* mqtt_server = "192.168.20.56";
const char* mqtt_username = "LegoDemonstrator";
const char* mqtt_password = "LegoDemonstrator1234";
const char* mqtt_client_id = "ESP32ImpressModul";

const char* mqtt_LED_cmd_topic = "/mqtt-key/LED002/cmd";
const char* mqtt_LED_cmdexe_topic = "/mqtt-key/LED002/cmdexe";
const char* mqtt_temperature_topic = "/mqtt-key/temperature002/attrs";

const int ledPin = 2;

//----------------------WiFi-------------------------------------------

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("\nConnected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

void initWifi() {
  // delete old config
  WiFi.disconnect(true);

  delay(1000);

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);

  /* Remove WiFi event
  Serial.print("WiFi Event ID: ");
  Serial.println(eventID);
  WiFi.removeEvent(eventID);*/

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    
  Serial.println();
  Serial.println();
  Serial.println("Wait for WiFi... ");
}

//------------------------JSON------------------------------------------------------------
DynamicJsonDocument jsonDoc(1024);

//-------------------------MQTT-----------------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float temperature = 0;

void initMQTT(){
  client.setServer(mqtt_server,1883);
  client.setCallback(mqtt_callback);
}

void mqtt_callback(char* topic, byte* message, unsigned int length){
  String messageTemp = "";
  String responseMessage = ""; 
  
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(" . Message: ");

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  jsonDoc.clear();
  auto error = deserializeJson(jsonDoc, messageTemp);
  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }

  if(String(topic) == mqtt_LED_cmd_topic){
    String LED_on = jsonDoc["on"];
    String LED_off = jsonDoc["off"];
    Serial.print("LED_on: ");
    Serial.println(LED_on);
    Serial.print("LED_off: ");
    Serial.println(LED_off);

    if(LED_on != "null"){
      if(LED_on == ""){
        jsonDoc["on"] = "LED is on!";
        digitalWrite(ledPin, HIGH);
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
        digitalWrite(ledPin, LOW);
      }
      else{
        jsonDoc["off"] = "Message body should be empty !";
      }
      serializeJson(jsonDoc, responseMessage);
      client.publish(mqtt_LED_cmdexe_topic, responseMessage.c_str());
    }
  }
}

void mqtt_reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection . ");
    if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
      Serial.println(" -> connected");
      // Subscribe
      client.subscribe(mqtt_LED_cmd_topic);
      client.subscribe(mqtt_LED_cmdexe_topic);
      client.subscribe(mqtt_temperature_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//------------------------Main------------------------------------------------------------
void setup(){
  Serial.begin(115200);
  initWifi();
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(" . ");
  }
  initMQTT();

  pinMode(ledPin, OUTPUT);
}

void loop(){
  if(!client.connected()) mqtt_reconnect();
  client.loop();

  long now = millis();
  if(now - lastMsg > 5000){
    lastMsg = now;
    String responseMessage = ""; 

    temperature = random(0,45);
    jsonDoc.clear();    
    jsonDoc["temperature"] = temperature;
    serializeJson(jsonDoc, responseMessage);
    client.publish(mqtt_temperature_topic, responseMessage.c_str());
  }
}