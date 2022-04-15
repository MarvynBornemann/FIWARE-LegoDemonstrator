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
    oledDisplay->setTextSize(2);             // Normal 1:1 pixel scale
    oledDisplay->setTextColor(SSD1306_WHITE);        // Draw white text
    oledDisplay->setCursor(0,0);
}

void OLED_Display::display() {
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    //draw Data on display
    oledDisplay->println("Dokk1"); 
    oledDisplay->println(" ");
    oledDisplay->println("Public");
    oledDisplay->println("Library"); 

    //update Display
    oledDisplay->display();
}

void OLED_Display::drawBitmap(int numberOfFreeParkingLots){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->drawBitmap(0, 0, Parking, 64, 64, SSD1306_WHITE);

    oledDisplay->setCursor(SCREEN_WIDTH/2,10);
    oledDisplay->print(" ");
    oledDisplay->println(numberOfFreeParkingLots);

    oledDisplay->setCursor(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 10);
    oledDisplay->setTextSize(1); 
    oledDisplay->println("  spaces");



    //update Display
    oledDisplay->display();
}