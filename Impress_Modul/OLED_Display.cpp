#include "OLED_Display.h"

OLED_Display::OLED_Display() {
    u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
    for(int i = 0; i < NUMBER_OF_LINES; i++){
        stringBuffer[i] = String("");
    }
}

OLED_Display::~OLED_Display() {
    delete u8g2;
}

void OLED_Display::setup() {
    u8g2->begin();
}

void OLED_Display::prepare() {
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->setFontRefHeightExtendedText();
    u8g2->setDrawColor(1);
    u8g2->setFontPosTop();
    u8g2->setFontDirection(0);
}

void OLED_Display::display(float weight, int weightClass, int price, int AvarageOfForceSensor1, int AvarageOfForceSensor2) {
    //clear and prepare Display
    u8g2->clearBuffer();
    prepare();

    //int to string
    String stringWeight = String(weight, 2);
    stringWeight = String(stringWeight + " t");
    String stringWeightClass = String(weightClass);
    String stringPrice = String(price);
    String stringAvarageOfForceSensor1 = String(AvarageOfForceSensor1);
    String stringAvarageOfForceSensor2 = String(AvarageOfForceSensor2);

    

    //draw Data on display
    u8g2->drawStr(0, 2, "Price: ");
    u8g2->drawStr(80, 2, stringPrice.c_str());
    u8g2->drawStr(110, 2, "/h");

    u8g2->drawStr(0, 15, "Weight Class");
    u8g2->drawStr(80, 15, stringWeightClass.c_str());

    u8g2->drawStr(0, 28, "Weight:");
    u8g2->drawStr(80, 28, stringWeight.c_str());

    u8g2->drawStr(0, 41, "Sensor 1:");
    u8g2->drawStr(80, 41, stringAvarageOfForceSensor1.c_str());

    u8g2->drawStr(0, 54, "Sensor 2:");
    u8g2->drawStr(80, 54, stringAvarageOfForceSensor2.c_str());

    u8g2->setFont(u8g2_font_unifont_t_symbols);
    u8g2->setFontPosTop();
    u8g2->setFontDirection(0);
    u8g2->drawUTF8(100, 0, "€");

    
    //update Display
    u8g2->sendBuffer();
}

void OLED_Display::println(String string) {
    //clear and prepare Display
    u8g2->clearBuffer();
    prepare();

    //append to String Buffer
    stringBuffer[bufferIndex] = string;

    for(int i = 0; i < NUMBER_OF_LINES; i++){
        u8g2->drawStr(0, 2 + i*13, stringBuffer[(NUMBER_OF_LINES-i + bufferIndex)%NUMBER_OF_LINES].c_str());
    }

    bufferIndex++;
    if(bufferIndex == NUMBER_OF_LINES) bufferIndex = 0;

    //update Display
    u8g2->sendBuffer();
}