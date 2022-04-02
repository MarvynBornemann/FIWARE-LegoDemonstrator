#include <Arduino.h>
#include <Grove_LED_Bar.h>
#include <Ultrasonic.h>
#include <stdlib.h>
#include <SoftwareSerial.h>
#include <SeeedOLED.h>
#include <Wire.h>

//Communication with ESP-01
SoftwareSerial ArduinoUno(10,11);

//Ultrasonic1
const int trigPin1 = 8;
const int echoPin1 = 7;

//Ultrasonic2
const int trigPin2 = 6;
const int echoPin2 = 5;

//Ultrasonic3
const int trigPin3 = 4;
const int echoPin3 = 3;

long duration1;
int distance1;

long duration2;
int distance2;

long duration3;
int distance3;

void setup() {
    ArduinoUno.begin(9600);
	pinMode(10,INPUT);
	pinMode(11,OUTPUT);
    pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
    pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin2, INPUT); 
    pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin3, INPUT); 
    SeeedOled.init();
    Wire.begin();
    //Serial.begin(9600); // Starts the serial communication
}

int p1=0,p2=0,p3=0;
float i1=0,i2=0,i3=0;
String us1s = "Slot 1 Free";
String us2s = "Slot 2 Free";
String us3s = "Slot 3 Free";


void loop() {
// Clears the trigPin
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1= duration1*0.034/2;
    //Serial.print("Distance1: ");
    //Serial.println(distance1);

    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
    distance2= duration2*0.034/2;

    digitalWrite(trigPin3, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin3, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin3, LOW);
    duration3 = pulseIn(echoPin3, HIGH);
    distance3= duration3*0.034/2;


    if(distance1 < 10) p1=1; else p1=0;
    if(distance2 < 10) p2=1; else p2=0;
    if(distance3 < 10) p3=1; else p3=0;

    if(p1==1) us1s = "Slot 1 Busy"; else us1s = "Slot 1 Free";
    if(p2==1) us2s = "Slot 2 Busy"; else us2s = "Slot 2 Free";
    if(p3==1) us3s = "Slot 3 Busy"; else us3s = "Slot 3 Free";

    ArduinoUno.print("| " + us1s + " : " + us2s + " : " + us3s + " |");
	  ArduinoUno.println("\n");
    SeeedOled.clearDisplay();          
    SeeedOled.setNormalDisplay();      
    SeeedOled.setPageMode();           
    SeeedOled.setTextXY(0, 0);         
    SeeedOled.putString("Status");
    SeeedOled.setTextXY(1, 0);
    SeeedOled.putString(String(us1s).c_str());
    SeeedOled.setTextXY(2, 0);
    SeeedOled.putString(String(us2s).c_str());
    SeeedOled.setTextXY(3, 0);
    SeeedOled.putString(String(us3s).c_str());
    
    delay(2000);
    
}