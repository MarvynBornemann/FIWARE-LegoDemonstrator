#pragma once

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

const unsigned char Parking[] PROGMEM = {// 'Parking', 64x64px
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x03, 
    0xc0, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 
    0xc0, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x03, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x0f, 0xfc, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x03, 0xfe, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x01, 0xfe, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0xfe, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0xff, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x7f, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x7f, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x7f, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x7f, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0xff, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0xff, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x01, 0xfe, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x03, 0xfe, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x07, 0xfc, 0x00, 0x03, 
    0xc0, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x03, 
    0xc0, 0x00, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 
    0xc0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x03, 
    0xc0, 0x00, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc
};

class OLED_Display
{
    private:
        Adafruit_SSD1306* oledDisplay;

        int sdaPin;
        int sclPin;
        int screenAdress;

        void prepare();

    public:
        OLED_Display(int sdaPin, int sclPin, int screenAdress = 0x3C);
        ~OLED_Display();
        
        void setup();

        void display();

        void drawBitmap(int numberOfFreeParkingLots);
};