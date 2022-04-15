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

void OLED_Display::displayElectricChargingSign(int numberOfFreeParkingLots){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->drawBitmap(0, 0, ElectricChargingSign, 64, 64, SSD1306_WHITE);

    oledDisplay->setCursor(SCREEN_WIDTH/2,10);
    oledDisplay->print(" ");
    oledDisplay->println(numberOfFreeParkingLots);

    oledDisplay->setCursor(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 10);
    oledDisplay->setTextSize(1); 
    oledDisplay->println("  spaces");

    //update Display
    oledDisplay->display();
}

void OLED_Display::displayElectricChargingProgress(bool parkingLot1_available, bool parkingLot2_available, int remainingTimeForFullCharge1, int remainingTimeForFullCharge2){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    //core setup of boat berths
    oledDisplay->drawRect(0,0,62,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(5,3);
    oledDisplay->print("1");
    oledDisplay->drawRect(66,0,62,SCREEN_HEIGHT, SSD1306_WHITE);
    oledDisplay->setCursor(71,3);
    oledDisplay->print("2");

    //draw sailboat if boat berths is not available
    if(!parkingLot1_available){
        oledDisplay->drawBitmap(30,3,Hourglass,32,32,SSD1306_WHITE);
        oledDisplay->setCursor(5,35);
        oledDisplay->print(remainingTimeForFullCharge1);
        oledDisplay->print(" s");
    }
    if(!parkingLot2_available){
        oledDisplay->drawBitmap(96,3,Hourglass,32,32,SSD1306_WHITE);
        oledDisplay->setCursor(71,35);
        oledDisplay->print(remainingTimeForFullCharge2);
        oledDisplay->print(" s");
    }

    //update Display
    oledDisplay->display();
}