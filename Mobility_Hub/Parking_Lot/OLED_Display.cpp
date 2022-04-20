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

void OLED_Display::displayParkingSign(int numberOfFreeParkingLots){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->drawBitmap(0, 0, Parking, 64, 64, SSD1306_WHITE);

    oledDisplay->setCursor(SCREEN_WIDTH/2,20);
    oledDisplay->print(" ");
    oledDisplay->println(numberOfFreeParkingLots);

    oledDisplay->setCursor(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 20);
    oledDisplay->setTextSize(1); 
    oledDisplay->println("  spaces");

    //update Display
    oledDisplay->display();
}

void OLED_Display::displayParkingLots(bool parkingLot1_available, bool parkingLot2_available, bool parkingLot3_available, bool parkingLot4_available){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    //core setup of boat berths
    oledDisplay->drawRect(0,0,31,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(10,2);
    oledDisplay->print("1");
    oledDisplay->drawRect(32,0,31,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(42,2);
    oledDisplay->print("2");
    oledDisplay->drawRect(64,0,31,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(74,2);
    oledDisplay->print("3");
    oledDisplay->drawRect(96,0,31,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(106,2);
    oledDisplay->print("4");

    //draw sailboat if boat berths is not available
    if(!parkingLot1_available){
        oledDisplay->drawBitmap(1,30,Car,29,29,SSD1306_WHITE);
    }
    if(!parkingLot2_available){
        oledDisplay->drawBitmap(33,30,Car,29,29,SSD1306_WHITE);
    }
    if(!parkingLot3_available){
        oledDisplay->drawBitmap(65,30,Car,29,29,SSD1306_WHITE);
    }
    if(!parkingLot4_available){
        oledDisplay->drawBitmap(97,30,Car,29,29,SSD1306_WHITE);
    }

    //update Display
    oledDisplay->display();
}