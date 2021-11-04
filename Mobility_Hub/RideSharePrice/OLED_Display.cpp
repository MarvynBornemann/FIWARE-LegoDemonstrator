#include "OLED_Display.h"

OLED_Display::OLED_Display(int sdaPin, int sclPin, int screenAdress) {
    this->sdaPin = sdaPin;
    this->sclPin = sclPin;
    this->screenAdress = screenAdress;
    oledDisplay = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

OLED_Display::~OLED_Display() {
    delete oledDisplay;
}

void OLED_Display::setup() {
    Wire.begin(sdaPin, sclPin);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!oledDisplay->begin(SSD1306_SWITCHCAPVCC, screenAdress)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    oledDisplay->display();
}

void OLED_Display::prepare() {
    oledDisplay->setTextSize(2);
    oledDisplay->setTextColor(SSD1306_WHITE);
    oledDisplay->setCursor(0,0);
}

void OLED_Display::displayBikeSharing(float priceBikeSharing){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->drawBitmap(0, 0, BikeSharing_bits, 64, 64, SSD1306_WHITE);

    oledDisplay->setCursor(SCREEN_WIDTH/2,20);
    oledDisplay->println(" " + String(priceBikeSharing,2));

    oledDisplay->drawBitmap(SCREEN_WIDTH/2 + 10,SCREEN_HEIGHT/2 + 10, Euro_bits, 16, 16, SSD1306_WHITE);
    oledDisplay->setCursor(SCREEN_WIDTH/2 + 30,SCREEN_HEIGHT/2 + 10);
    oledDisplay->println("/h");

    //update Display
    oledDisplay->display();
}

void OLED_Display::displayCarSharing(float priceCarSharing){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->drawBitmap(0, 0, CarSharing_bits, 64, 64, SSD1306_WHITE);

    oledDisplay->setCursor(SCREEN_WIDTH/2,20);
    oledDisplay->println(String(priceCarSharing,2));

    oledDisplay->drawBitmap(SCREEN_WIDTH/2 + 10,SCREEN_HEIGHT/2 + 10, Euro_bits, 16, 16, SSD1306_WHITE);
    oledDisplay->setCursor(SCREEN_WIDTH/2 + 30,SCREEN_HEIGHT/2 + 10);
    oledDisplay->println("/h");

    //update Display
    oledDisplay->display();
}

void OLED_Display::displayParticulateMatter(float particulateMatter){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->drawBitmap(0, 10, AirQuality_bits, 64, 64, SSD1306_WHITE);

    oledDisplay->setTextSize(1); 
    oledDisplay->println("Particulate Matter");

    oledDisplay->drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    oledDisplay->setTextSize(2); 
    oledDisplay->setCursor(SCREEN_WIDTH/2,20);
    oledDisplay->println(String(particulateMatter,1));

    oledDisplay->drawBitmap(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 14, My_bits, 10, 16, SSD1306_WHITE);
    oledDisplay->setCursor(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 10);
    oledDisplay->println(" g/m3");

    //update Display
    oledDisplay->display();
}