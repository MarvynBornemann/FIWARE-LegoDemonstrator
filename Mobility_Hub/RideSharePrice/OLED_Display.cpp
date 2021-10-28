#include "OLED_Display.h"

OLED_Display::OLED_Display() {
    u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, ESP8266_SCL_PIN, ESP8266_SDA_PIN);
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

void OLED_Display::display(float particulateMatter, int priceCarSharing, int priceBikeSharing) {
    //clear and prepare Display
    u8g2->clearBuffer();
    prepare();

    //int to string
    String stringParticulateMatter = String(particulateMatter,1);
    String stringPriceCarSharing = String(priceCarSharing);
    String stringPriceBikeSharing = String(priceBikeSharing);

    //draw Data on display
    u8g2->drawStr(0, 0, "Particulate Matter");

    u8g2->drawStr(0,12, "Level: ");
    u8g2->drawStr(60,12, stringParticulateMatter.c_str());
    u8g2->drawStr(100,12, "g/m3");

    u8g2->drawLine(0, 24, 128, 24);

    u8g2->drawStr(0,26, "Prices: ");

    u8g2->drawStr(0,38, "Car Sharing: ");
    u8g2->drawStr(90,38, stringPriceCarSharing.c_str());
    u8g2->drawStr(110,38, "/h");

    u8g2->drawStr(0,50, "Bike Sharing:");
    u8g2->drawStr(90,50, stringPriceBikeSharing.c_str());
    u8g2->drawStr(110,50, "/h");

    u8g2->setFont(u8g2_font_unifont_t_symbols);
    u8g2->setFontPosTop();
    u8g2->setFontDirection(0);
    u8g2->drawUTF8(90, 10, "µ");
    u8g2->drawUTF8(102, 36, "€");
    u8g2->drawUTF8(102, 48, "€");

    //update Display
    u8g2->sendBuffer();
}

void OLED_Display::drawBitmap(){
    //clear and prepare Display
    u8g2->clearBuffer();
    //prepare();

    u8g2->setBitmapMode(false /* solid */);
    u8g2->setDrawColor(1);

    u8g2->drawXBM(0,0,BikeCarSharing_width,BikeCarSharing_height,BikeCarSharing_bits);

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