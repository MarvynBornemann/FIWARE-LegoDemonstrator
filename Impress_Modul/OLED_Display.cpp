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

void OLED_Display::display(float weight, int price) {
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();

    oledDisplay->setCursor(0,16);
    oledDisplay->println(" " + String(weight,2) + " t");

    oledDisplay->setCursor(0,SCREEN_HEIGHT/2 + 18);
    oledDisplay->println(" " + String(price));

    oledDisplay->drawBitmap(SCREEN_WIDTH/2 + 10,SCREEN_HEIGHT/2 + 16, Euro_bits, 16, 16, SSD1306_WHITE);
    oledDisplay->setCursor(SCREEN_WIDTH/2 + 30,SCREEN_HEIGHT/2 + 18);
    oledDisplay->println("/h");

    oledDisplay->setTextSize(1);
    oledDisplay->setCursor(0,0);
    oledDisplay->println(" weight:");
    oledDisplay->setCursor(0,SCREEN_HEIGHT/2 + 5);
    oledDisplay->println(" price:");

    oledDisplay->drawLine(0,SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, SSD1306_WHITE);

    //update Display
    oledDisplay->display();
}