#pragma once

#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>

const char COPYRIGHT_SYMBOL[] = { 0xa9, '\0' };

class OLED_Display
{
    private:
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2;

        void renderScreen();

    public:
        OLED_Display();
        ~OLED_Display();
        
        void setup();
        void prepare();

        void display(int weight, int weightClass, int AvarageOfForceSensor1, int AvarageOfForceSensor2);
};