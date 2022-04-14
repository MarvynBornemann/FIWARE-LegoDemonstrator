## 00 General Info
# The hackathon setup to control the Vienna Module is following the tutorial setup from the NGSI-LD / IoT Agent tutorial to be found here: https://ngsi-ld-tutorials.readthedocs.io/en/latest/iot-agent.html. 
# 
# The Vienna Module is a remote edge device to be controlled via the Orion-LD Context Broker. There is a MQTT Broker and an IoT Agent working in the background, both are not and must not be accessible to the hackathon participants. All setup activities are done and the participants can start to control the Vienna LEGO module only with NGSI-LD interactions with the Orion-LD Context Broker.
# 
# The connection to the Vienna Module is unidirectional. You can only control elements on/to the Vienna Module. There are no sensor readings realized from the Vienna Module so far. 
# 
# For controlling the LEGO module you just have to make a PATCH HTTP-Call as shown below. You can find a working demo call for all available commands. Additional details can be found in the individual call descriptions. It is following the descriptions to be found here: https://ngsi-ld-tutorials.readthedocs.io/en/latest/iot-agent-json.html#southbound-traffic-commands
# 
# To start a control command just release one of the following PATCH HTTP-Call-Commands to update the individual command on the Vienna Module. Please look out for logical sequences of in the individual descriptions and do not inject commands on a high fequency. Best wait 5-10 seconds before releasing next calls, the Vienna Module is build from LEGO Bricks and is a toy, not an industrial application. 
# 
# For each of the existing commands you will find 2 Attributes in the Entity named "urn:ngsi-ld:Device:vienna-legomodule-001", i.e. Command HOUSE_DEMO -> ENABLE
# HOUSE_DEMO_status should show the value "OK" for successful command execution 
# HOUSE_DEMO_info should show a command confirmation like: "HOUSE_DEMO ENABLED"
# 
# And please be cautious when controlling the Vienna Module, do not abuse it. 
curl "https://ngsi-ld-tutorials.readthedocs.io/en/latest/iot-agent.html"

## 01 Check Entitiy and Attribute values
# Read the control values from the urn:ngsi-ld:Device:vienna-legomodule-001 NGSI-LD-Entity.
curl "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001" \
     -H 'Fiware-Service: openiot' \
     -H 'Fiware-Servicepath: /' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"'

## 02 Update HOUSE_DEMO -> ENABLE
# Enable automatic changes of the town hall lightings.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/HOUSE_DEMO" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "ENABLE"
}'

## 03 Update HOUSE_DEMO -> DISABLE
# Disable automatic changes of the town hall lightings.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/HOUSE_DEMO" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "DISABLE"
}'

## 04 Update HOUSE_RGB -> 3 RGB color values
# Please always disable the automatic changes of the town hall lightings with call "03 Update HOUSE_DEMO -> DISABLE" before using this command to avoid strange behaviour. 
# 
# For changing the town hall lightings with your own mixed colors, please change the payload of this PATCH command to your individual RGB values. Values are to be named in R G B sequence separated with one space between the values. Values have to be between 0 and 255.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/HOUSE_RGB" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "111 112 113"
}'

## 05 Update HOUSE_R -> 1 RGB color R value
# Please always disable the automatic changes of the town hall lightings with call "03 Update HOUSE_DEMO -> DISABLE" before using this command to avoid strange behaviour. 
# 
# For changing the town hall lightings with your own R color value, please change the payload of this PATCH command to your individual R value. This call is only to address the R value in the range between 0 and 255.
# 
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/HOUSE_R" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "111"
}'

## 06 Update HOUSE_G -> 1 RGB color G value
# Please always disable the automatic changes of the town hall lightings with call "03 Update HOUSE_DEMO -> DISABLE" before using this command to avoid strange behaviour. 
# 
# For changing the town hall lightings with your own G color value, please change the payload of this PATCH command to your individual G value. This call is only to address the G value in the range between 0 and 255.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/HOUSE_G" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "112"
}'

## 07 Update HOUSE_B -> 1 RGB color B value
# Please always disable the automatic changes of the town hall lightings with call "03 Update HOUSE_DEMO -> DISABLE" before using this command to avoid strange behaviour. 
# 
# For changing the town hall lightings with your own B color value, please change the payload of this PATCH command to your individual B value. This call is only to address the B value in the range between 0 and 255.
# 
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/HOUSE_B" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "113"
}'

## 08 Update FERRIS -> ENABLE
# Enable turning of the Ferris wheel.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/FERRIS" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "ENABLE"
}'

## 09 Update FERRIS -> DISABLE
# Disable turning of the Ferris wheel.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/FERRIS" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "DISABLE"
}'

## 10 Update METRO_DEMO -> ENABLE
# Enable automatic operation of the 2 metro lines 1 and 2.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/METRO_DEMO" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "ENABLE"
}'

## 11 Update METRO_DEMO -> DISABLE
# Disable automatic operation of the 2 metro lines 1 and 2.
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/METRO_DEMO" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "DISABLE"
}'

## 12 Update METRO_1_DRIVE_Station - go to Station
# Please always disable the automatic operation of the METRO Demo with call "11 Update METRO_DEMO -> DISABLE" before using this command to avoid strange behaviour. 
# 
# Command Metro 1 to drive into the station to stop and wait there.
# 
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/METRO_1_DRIVE" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "INTO_STATION"
}'

## 13 Update METRO_1_DRIVE_Tunnel - go to Tunnel
# Please always disable the automatic operation of the METRO Demo with call "11 Update METRO_DEMO -> DISABLE" before using this command to avoid strange behaviour.  
# 
# Command Metro 1 to drive into the tunnel to stop and wait there.
# 
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/METRO_1_DRIVE" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "INTO_TUNNEL"
}'

## 14 Update METRO_2_DRIVE_Station - go to Station
# Please always disable the automatic operation of the METRO Demo with call "11 Update METRO_DEMO -> DISABLE" before using this command to avoid strange behaviour. 
# 
# Command Metro 2 to drive into the station to stop and wait there.
# 
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/METRO_2_DRIVE" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "INTO_STATION"
}'

## 15 Update METRO_2_DRIVE_Tunnel - go to Tunnel
# Please always disable the automatic operation of the METRO Demo with call "11 Update METRO_DEMO -> DISABLE" before using this command to avoid strange behaviour. 
# 
# 
# Command Metro 2 to drive into the tunnel to stop and wait there.
# 
curl -X "PATCH" "http://46.17.108.109:1026/ngsi-ld/v1/entities/urn:ngsi-ld:Device:vienna-legomodule-001/attrs/METRO_2_DRIVE" \
     -H 'NGSILD-Tenant: openiot' \
     -H 'Content-Type: application/json' \
     -H 'Link: <http://context/ngsi-context.jsonld>; rel="http://www.w3.org/ns/json-ld#context"; type="application/ld+json"' \
     -d $'{
  "type": "Property",
  "value": "INTO_TUNNEL"
}'
