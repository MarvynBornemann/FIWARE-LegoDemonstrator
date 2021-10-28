#include "OLED_Display.h"

OLED_Display::OLED_Display() {
    u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, WIO_NODE_SCL_PIN, WIO_NODE_SDA_PIN);
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

void OLED_Display::display(int numberOfFreeParkingLots) {
    //clear and prepare Display
    u8g2->clearBuffer();
    prepare();

    //int to string
    String stringNumberOfFreeParkingLots = String(numberOfFreeParkingLots);  

    //draw Data on display
    u8g2->drawStr(0, 2, "Dokk1");

    u8g2->drawStr(0,16, "Public Library");

    u8g2->drawLine(0, 30, 128, 30);

    u8g2->drawStr(0,34, "Parking spaces");

    u8g2->drawStr(0,48, "available:");
    u8g2->drawStr(80,48, stringNumberOfFreeParkingLots.c_str());

    //update Display
    u8g2->sendBuffer();
}

void OLED_Display::drawBitmap(){
    //clear and prepare Display
    u8g2->clearBuffer();
    //prepare();

    u8g2->setBitmapMode(false /* solid */);
    u8g2->setDrawColor(0);

    u8g2->drawXBM(32,0,64,64,Parking);

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