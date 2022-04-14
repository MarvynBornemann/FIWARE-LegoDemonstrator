#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = "FIWARE";
const char* password = "!FIWARE!on!air!";

const char* mqtt_server = "46.17.108.109";
const char* mqtt_user = "hackathon";
const char* mqtt_password = "hackathon1234";
const char* mqtt_client_id = "M5StackAtomMatrix";

const char* mqtt_cmd_topic = "/1f01eb1c-e654-11eb-ba80-0242ac130004/vienna-legomodule-001/cmd";
const char* mqtt_cmdexe_topic = "/1f01eb1c-e654-11eb-ba80-0242ac130004/vienna-legomodule-001/cmdexe";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(" . ");
  }

  Serial.print(" -> WiFi connected");
  Serial.print(" -> IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* message, unsigned int length) {

  String messageTemp = "";
  String responseMessage = ""; 
  
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(" . Message: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  String deviceID = messageTemp.substring (0, messageTemp.indexOf ('@'));
  String command = messageTemp.substring (messageTemp.indexOf ('@') + 1, messageTemp.indexOf ('|'));
  String messageBody = messageTemp.substring (messageTemp.indexOf ('|') + 1);
  
  String responseMessageBase = deviceID + '@' + command + '|';
  String responseMessageComplete = "";
  
  Serial.println("DeviceID extracted: " + deviceID);
  Serial.println("Command extracted: " + command);
  Serial.println("MessageBody extracted: " + messageBody);
  
  if (String(topic) == mqtt_cmd_topic) {

    // LED command, only LED BLINK
    if(command == "LED" && messageBody == "LED BLINK"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "LED is blinking";
      client.publish(mqtt_cmdexe_topic, (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "LED"){
      responseMessageComplete = responseMessageBase + "Processing LED command failed, messagebody " + messageBody + " <> LED BLINK";
      client.publish(mqtt_cmdexe_topic, (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }


    // HOUSE_DEMO command, ENABLE or NOT ENABLE
    else if(command == "HOUSE_DEMO" && messageBody == "ENABLE"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "HOUSE_DEMO ENABLED";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "HOUSE_DEMO"){
      client.publish( (char*) command.c_str(), "DISABLE");
      responseMessageComplete = responseMessageBase + "HOUSE_DEMO DISABLED, messagebody " + messageBody + " <> ENABLE";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }


    // HOUSE_RGB command, 3 Bytes
    else if(command == "HOUSE_RGB"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "HOUSE_RGB set to: " + messageBody;
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }


    // HOUSE_R command, 1 Bytes
    else if(command == "HOUSE_R"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "HOUSE_R set to: " + messageBody;
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }

    
    // HOUSE_G command, 1 Bytes
    else if(command == "HOUSE_G"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "HOUSE_G set to: " + messageBody;
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }


    // HOUSE_B command, 1 Bytes
    else if(command == "HOUSE_B"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "HOUSE_B set to: " + messageBody;
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }

    
    // FERRIS command, ENABLE or NOT ENABLE
    else if(command == "FERRIS" && messageBody == "ENABLE"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "FERRIS ENABLED";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "FERRIS"){
      client.publish( (char*) command.c_str(), "DISABLE");
      responseMessageComplete = responseMessageBase + "FERRIS DISABLED, messagebody " + messageBody + " <> ENABLE";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }

    
    // METRO_DEMO command, ENABLE or NOT ENABLE
    else if(command == "METRO_DEMO" && messageBody == "ENABLE"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "METRO_DEMO ENABLED";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "METRO_DEMO"){
      client.publish( (char*) command.c_str(), "DISABLE");
      responseMessageComplete = responseMessageBase + "METRO_DEMO DISABLED, messagebody " + messageBody + " <> ENABLE";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }

    
    // METRO_1_DRIVE command, INTO_STATION or INTO_TUNNEL or else
    else if(command == "METRO_1_DRIVE" && messageBody == "INTO_STATION"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "METRO_1_DRIVE INTO_STATION";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "METRO_1_DRIVE" && messageBody == "INTO_TUNNEL"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "METRO_1_DRIVE INTO_TUNNEL";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "METRO_1_DRIVE"){
      responseMessageComplete = responseMessageBase + "Processing METRO_1_DRIVE command failed, messagebody " + messageBody + " <> INTO_STATION or INTO_TUNNEL";
      client.publish(mqtt_cmdexe_topic, (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }

    
    // METRO_2_DRIVE command, INTO_STATION or INTO_TUNNEL or else
    else if(command == "METRO_2_DRIVE" && messageBody == "INTO_STATION"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "METRO_2_DRIVE INTO_STATION";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "METRO_2_DRIVE" && messageBody == "INTO_TUNNEL"){
      client.publish( (char*) command.c_str(), (char*) messageBody.c_str());
      responseMessageComplete = responseMessageBase + "METRO_2_DRIVE INTO_TUNNEL";
      client.publish(mqtt_cmdexe_topic,  (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
    else if(command == "METRO_2_DRIVE"){
      responseMessageComplete = responseMessageBase + "Processing METRO_2_DRIVE command failed, messagebody " + messageBody + " <> INTO_STATION or INTO_TUNNEL";
      client.publish(mqtt_cmdexe_topic, (char*) responseMessageComplete.c_str());
      Serial.println("ResponseMessage: " + responseMessageComplete);
      Serial.println("---");
    }
  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection . ");
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_password)) {
      Serial.println(" -> connected");
      // Subscribe
      client.subscribe(mqtt_cmd_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 500) {
    lastMsg = now;
  }
}
