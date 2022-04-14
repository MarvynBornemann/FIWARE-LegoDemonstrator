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

void OLED_Display::displayParkingSign(int numberOfFreeBoatBerths){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->drawBitmap(0, 0, Parking, 64, 64, SSD1306_WHITE);

    oledDisplay->setCursor(SCREEN_WIDTH/2,10);
    oledDisplay->print(" ");
    oledDisplay->println(numberOfFreeBoatBerths);

    oledDisplay->setCursor(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 10);
    oledDisplay->setTextSize(1); 
    oledDisplay->println("  spaces");

    //update Display
    oledDisplay->display();
}

void OLED_Display::displayBoatBerths(bool boatBerth1_available, bool boatBerth2_available, bool boatBerth3_available){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    //core setup of boat berths
    oledDisplay->drawRect(0,0,40,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(16,3);
    oledDisplay->print("1");
    oledDisplay->drawRect(44,0,40,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(60,3);
    oledDisplay->print("2");
    oledDisplay->drawRect(88,0,40,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(104,3);
    oledDisplay->print("3");

    //draw sailboat if boat berths is not available
    if(!boatBerth1_available){
        oledDisplay->drawBitmap(1,19,Sailboat,38,44,SSD1306_WHITE);
    }
    if(!boatBerth2_available){
        oledDisplay->drawBitmap(45,19,Sailboat,38,44,SSD1306_WHITE);
    }
    if(!boatBerth3_available){
        oledDisplay->drawBitmap(89,19,Sailboat,38,44,SSD1306_WHITE);
    }

    //update Display
    oledDisplay->display();
}