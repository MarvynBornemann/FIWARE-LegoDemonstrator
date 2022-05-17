#pragma once

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

const unsigned char Euro_bits[] PROGMEM = {// 'Euro', 16x16px
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0xfc, 0x06, 0x00, 0x0e, 0x00, 0x3f, 0xc0, 0x0c, 0x00, 
    0x0c, 0x00, 0x3f, 0xc0, 0x0e, 0x00, 0x06, 0x00, 0x03, 0xfc, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00
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

        void display(float weight, int price);
};