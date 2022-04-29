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

void OLED_Display::display(float energy, String energyType) {
    Wire.begin(sdaPin, sclPin);
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();  

    //draw Data on display
    oledDisplay->println("Power");
    oledDisplay->println(energyType);
    oledDisplay->drawLine(0, SCREEN_HEIGHT/2+3, SCREEN_WIDTH, SCREEN_HEIGHT/2+3, SSD1306_WHITE);
    oledDisplay->println("");
    oledDisplay->print(String(energy,1));
    oledDisplay->println(" kW");

    //update Display
    oledDisplay->display();
}

void OLED_Display::drawBitmap(const unsigned char* Bitmap){
    Wire.begin(sdaPin, sclPin);
    //clear and prepare Display
    oledDisplay->clearDisplay();
    //prepare();

    oledDisplay->drawBitmap(32, 0, Bitmap, 64, 64, SSD1306_WHITE);

    //update Display
    oledDisplay->display();
}