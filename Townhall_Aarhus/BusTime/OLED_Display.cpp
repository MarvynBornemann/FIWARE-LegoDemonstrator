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
    oledDisplay->setTextSize(1);             // Normal 1:1 pixel scale
    oledDisplay->setTextColor(SSD1306_WHITE);        // Draw white text
    oledDisplay->setCursor(0,0);
}

void OLED_Display::display() {
    //clear and prepare Display
    oledDisplay->clearDisplay();
    prepare();  

    //draw Data on display
    oledDisplay->println("Route  Goal      Time");
    oledDisplay->drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
    oledDisplay->println(" ");

    int oldBusIndex = busIndex;
    for(int i = 0; i < numberOfBus; i++){
        int index = (oldBusIndex + i)%numberOfBus;
        int busTime = busTimes[index] - time;
        if(busTime == 0) busIndex++;
        if(busTime < 0) busTime += 20;
        oledDisplay->print(busRoutes[index]);
        oledDisplay->print(" ");
        oledDisplay->print(busDestination[index]);
        oledDisplay->print(" ");
        oledDisplay->println(busTime);
        oledDisplay->println(" ");
    }

    //update Display
    oledDisplay->display();

    time++;
    if(time > 19) time = 0;
    if(busIndex > numberOfBus) busIndex = 0;
    subStringIndex = 0;
}

void OLED_Display::displayLarge(){
    unsigned long currentTime = millis();
    unsigned long diffTime = currentTime - lastTime;

    if(diffTime > 1000){
        lastTime = currentTime;
    
        //clear and prepare Display
        oledDisplay->clearDisplay();
        prepare();
        oledDisplay->setTextSize(2);  

        //draw Data on display
        int busTime = busTimes[busIndex] - time;
        oledDisplay->print("Route ");
        oledDisplay->println(busRoutes[busIndex]);

        oledDisplay->println(busDestination[busIndex].substring(subStringIndex, subStringIndex + 10));
        oledDisplay->println(" ");

        oledDisplay->print(busTime);
        oledDisplay->println(" min");

        //update Display
        oledDisplay->display();
        subStringIndex++;
        if(subStringIndex > 3) subStringIndex = 0;
    }
}

void OLED_Display::drawBitmap(){
    //clear and prepare Display
    oledDisplay->clearDisplay();
    //prepare();

    oledDisplay->drawBitmap(0, 0, Bus, 128, 64, SSD1306_WHITE);

    //update Display
    oledDisplay->display();
}