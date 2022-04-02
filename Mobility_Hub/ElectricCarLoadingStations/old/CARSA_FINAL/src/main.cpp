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
const int trigPin1 = 9;
const int echoPin1 = 8;

//Ultrasonic2
const int trigPin2 = 5;
const int echoPin2 = 4;

Ultrasonic ultrasonic3(A3);
Ultrasonic ultrasonic4(A1);

Grove_LED_Bar bar1(7, 6, 0);
Grove_LED_Bar bar3(6, 5, 0);
Grove_LED_Bar bar2(3, 2, 0);
Grove_LED_Bar bar4(1, 0, 0);

long duration1;
int distance1;

long duration2;
int distance2;

void setup() {
    ArduinoUno.begin(9600);
	pinMode(10,INPUT);
	pinMode(11,OUTPUT);
    pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
    pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin2, INPUT); 
    SeeedOled.init();
    Wire.begin();
    //Serial.begin(9600); // Starts the serial communication
}

int p1=0,p2=0,p3=0,p4=0;
float i1=0,i2=0,i3=0,i4=0;
String us1s = "Slot 1 Free";
String us2s = "Slot 2 Free";
String us3s = "Slot 3 Free";
String us4s = "Slot 4 Free";

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
    //Serial.print("Distance2: ");
    //Serial.println(distance2);

    int us3 = ultrasonic3.MeasureInInches(); 
    int us4 = ultrasonic4.MeasureInInches(); 
    //Serial.print("Distance3: ");
    //Serial.println(us3);
    //Serial.print("Distance4: ");
    //Serial.println(us4);
    p1=0,p2=0,p3=0,p4=0;

    if(distance1 < 10) p1=1;
    if(distance2 < 10) p2=1;
    if(us3 < 10) p3=1;
    if(us4 < 10) p4=1;
    char buffer[40];
    bar1.setGreenToRed(false);
    bar2.setGreenToRed(false);
    bar3.setGreenToRed(false);

    if(p1==1)
    {    
        bar1.setLevel(i1);
        if(i1<10.1)
        {
            us1s = itoa(i1*10,buffer,10);
            us1s= us1s + "% charged";
            i1=i1+1;
        }
        if(i1>10)
        {
            us1s = "Car 1 Available";
        }
    }

    if(p2==1)
    {    
        bar2.setLevel(i2);
        if(i2<10.1)
        {
            us2s = itoa(i2*10,buffer,10);
            us2s= us2s + "% charged";
            i2=i2+1;
        }
        if(i2>10)
        {
            us2s = "Car 2 Available";
        }
    }

    if(p3==1)
    {    
        bar3.setLevel(i3);
        if(i3<10.1)
        {
            us3s = itoa(i3*10,buffer,10);
            us3s= us3s + "% charged";
            i3=i3+1;
        }
        if(i3>10)
        {
            us3s = "Car 3 Available";
        }
    }

    if(p4==1)
    {    
        bar4.setLevel(i4);
        if(i4<10.1)
        {
            us4s = itoa(i4*10,buffer,10);
            us4s= us4s + "% charged";
            i4=i4+1;
        }
        if(i4>10)
        {
            us4s = "Car 4 Available";
        }
    }


    if(p1==0)
    { 
        i1=0;
        bar1.setLevel(i1);
        us1s = "Slot 1 Free";
    }
    if(p2==0)
    {   
        i2=0;
        bar2.setLevel(i2);
        us2s = "Slot 2 Free";
    }
    if(p3==0)
    {   
        i3=0;
        bar3.setLevel(i3);
        us3s = "Slot 3 Free";
    }
    if(p4==0)
    {   
        i4=0;
        bar4.setLevel(i4);
        us4s = "Slot 4 Free";
    }
    ArduinoUno.print("| " + us1s + " : " + us2s + " : " + us3s + " : " + us4s + " |");
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
    SeeedOled.setTextXY(4, 0);
    SeeedOled.putString(String(us4s).c_str());
    
    delay(1000);
    
}