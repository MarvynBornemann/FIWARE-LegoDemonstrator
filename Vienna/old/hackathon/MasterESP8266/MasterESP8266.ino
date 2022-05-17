#include <stdint.h>

#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiManager.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>
#include <Wire.h>



// GPIO Pin 4 as SDA in ESP 8266
#define SDA_PIN 4

// GPIO Pin 5 as Sclk in ESP 8266
#define SCL_PIN 5



// Defining all the command bytes for the Atmega MCU, according to the Vienna Protocol document

#define TW_HOUSE_DEMO 0x20
#define TW_HOUSE_RGB 0x21
#define TW_HOUSE_R 0x22
#define TW_HOUSE_G 0x23
#define TW_HOUSE_B 0x24
#define TW_FERRIS_ENABLED 0x30
#define TW_METRO_DEMO 0x40
#define TW_METRO_1_DRIVE 0x41
#define TW_METRO_2_DRIVE 0x42

#define TW_DISABLE 0x00
#define TW_ENABLE 0x01
#define TW_INTO_STATION 0x00
#define TW_INTO_TUNNEL 0x01


/*
  //Defining parameters of MQTT
  const char* ssid = "TP-Link_B218";                     //Wifi SSid
  const char* password = "47626881";                     //Wifi Password
  const char* mqttServer = "192.168.0.102";              //IP address of Raspberry Pi which has my MQTT broker
  const int mqttPort = 1883;
  const char* mqttUser = "";                             //MQTT  USER id
  const char* mqttPassword = "";                         //MQTT Password

*/

/*
//Defining parameters of MQTT
const char* ssid = "LuxActiveKG_oha";                     //Wifi SSid
const char* password = "connectustooha";                     //Wifi Password
const char* mqttServer = "5.196.95.208";              //IP address of Raspberry Pi which has my MQTT broker
const int mqttPort = 1883;
const char* mqttUser = "";                             //MQTT  USER id
const char* mqttPassword = "";                         //MQTT Password
*/


//Defining parameters of MQTT
const char* ssid = "FIWARE_fair";                      //Wifi SSid
const char* password = "!FIWARE!on!air!";              //Wifi Password
const char* mqttServer = "46.17.108.109";              //IP address of Raspberry Pi which has my MQTT broker
const int mqttPort = 1883;
const char* mqttUser = "hackathon";                    //MQTT  USER id
const char* mqttPassword = "hackathon1234";            //MQTT Password



//Callback function called whenever I get any message from the MQTT broker to my client, i.e the ESP8266
void callback (const char * const char_topic, const byte * const char_payload, const unsigned int length)
{
  const String topic (char_topic);
  String message = "";

  for (int i = 0; i < length; i++)         //Printing character by character the message in the payload
  {
    message += (char)char_payload[i];
  }

  // parse bytes
  const byte byte1 = (byte) message.toInt ();
  const String rest1 = message.substring (message.indexOf (' ') + 1);
  const byte byte2 = (byte) rest1.toInt ();
  const String rest2 = rest1.substring (rest1.indexOf (' ') + 1);
  const byte byte3 = (byte) rest2.toInt ();

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  Serial.println(message);
  Serial.println(byte1);
  Serial.println(byte2);
  Serial.println(byte3);
  Serial.println("--------------------");

  if (topic == "LED")
  {
    if (message == "LED BLINK")
    {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(2000);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED on (HIGH is the voltage level)
      delay(5000);                       // wait for a second

      Serial.println("LED is blinking");
    }
    else
    {
      Serial.println("To blink LED, MQTT messages should be 'LED BLINK' in topic LED"); //
    }
  }
  else if (topic == "HOUSE_DEMO")
  {
    Serial.println ("TW: HOUSE_DEMO");
    Wire.beginTransmission(111);
    Wire.write(TW_HOUSE_DEMO);
    Wire.write((message == "ENABLE") ? TW_ENABLE : TW_DISABLE);
    Wire.endTransmission();
  }
  else if (topic == "HOUSE_RGB")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_HOUSE_RGB);
    Wire.write(byte1);
    Wire.write(byte2);
    Wire.write(byte3);
    Wire.endTransmission();
  }
  else if (topic == "HOUSE_R")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_HOUSE_R);
    Wire.write(byte1);
    Wire.endTransmission();
  }
  else if (topic == "HOUSE_G")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_HOUSE_G);
    Wire.write(byte1);
    Wire.endTransmission();
  }
  else if (topic == "HOUSE_B")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_HOUSE_B);
    Wire.write(byte1);
    Wire.endTransmission();
  }
  else if (topic == "FERRIS")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_FERRIS_ENABLED);
    Wire.write((message == "ENABLE") ? TW_ENABLE : TW_DISABLE);
    Wire.endTransmission();
  }
  else if (topic == "METRO_DEMO")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_METRO_DEMO);
    Wire.write((message == "ENABLE") ? TW_ENABLE : TW_DISABLE);
    Wire.endTransmission();
  }
  else if (topic == "METRO_1_DRIVE")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_METRO_1_DRIVE);

    if (message == "INTO_STATION")
    {
      Wire.write(TW_INTO_STATION);
    }
    else if (message == "INTO_TUNNEL")
    {
      Wire.write(TW_INTO_TUNNEL);
    }

    Wire.endTransmission();
  }
  else if (topic == "METRO_2_DRIVE")
  {
    Wire.beginTransmission(111);
    Wire.write(TW_METRO_2_DRIVE);

    if (message == "INTO_STATION")
    {
      Wire.write(TW_INTO_STATION);
    }
    else if (message == "INTO_TUNNEL")
    {
      Wire.write(TW_INTO_TUNNEL);
    }

    Wire.endTransmission();
  }
}




// Setting up MQTT
WiFiClient espClient;
PubSubClient client(mqttServer, mqttPort, callback, espClient);



void setup() {
  // join i2c bus with SDA=4 and SCL=5 of NodeMCU
  Wire.begin(SDA_PIN, SCL_PIN);

  // Beginning of printing in the serial monitor, according to the baudrate of NodeMCU i.e. 115200
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFI..");
    delay(500);
  }

  Serial.println("Connected to WiFi network");

  // Setup our MQTT server
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESPClient8266", mqttUser, mqttPassword))
    {
      Serial.println("Connected to MQTT");
      client.subscribe("LED");
      client.subscribe("HOUSE_DEMO");
      client.subscribe("HOUSE_RGB");
      client.subscribe("HOUSE_R");
      client.subscribe("HOUSE_G");
      client.subscribe("HOUSE_B");
      client.subscribe("FERRIS");
      client.subscribe("METRO_DEMO");
      client.subscribe("METRO_1_DRIVE");
      client.subscribe("METRO_2_DRIVE");
    }
    else {
      Serial.print("failed with state");
      Serial.print(client.state());
      delay(2000);
    }
  }
}



void loop()
{
  client.loop();
}
