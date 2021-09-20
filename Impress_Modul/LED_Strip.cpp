#include "LED_Strip.h"

LED_Strip::LED_Strip(int numberOfLEDs, int pinLED){
    strip = new Adafruit_NeoPixel(numberOfLEDs, pinLED, NEO_GRB + NEO_KHZ800);
    _lastTime = millis();
    pixelIndex = 0;
    mode = 0;
}

LED_Strip::~LED_Strip(){
    delete strip;
}

void LED_Strip::setup(){
    strip->begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip->show();            // Turn OFF all pixels ASAP
    strip->setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void LED_Strip::loop(){
    // Fill along the length of the strip in various colors...
    switch(mode){
        case 0:
            plainColor(strip->Color(255,   255,   255), 1000, 2); // White
            break;
        case 1:
            colorWipe(strip->Color(255,   0,   0), 50, 2); // Red
            break;
        case 2:
            colorWipe(strip->Color(  0, 255,   0), 50, 2); // Green
            break;
        case 3:
            colorWipe(strip->Color(  0,   0, 255), 50, 2); // Blue
            break;
        case 4:
            rainbow(10, 5);             // Flowing rainbow cycle along the whole strip
            break;
        case 5:
            theaterChase(strip->Color(127,   0,   0), 50, 10); // Red, half brightness
            break;
        case 6:
            theaterChase(strip->Color(127, 127, 127), 50, 10); // White, half brightness
            break;
        case 7: 
            theaterChaseRainbow(50, 30); // Rainbow-enhanced theaterChase variant
            break;
        default:
            strip->clear();
            strip->show();
            break;
    }
}

bool LED_Strip::nextPixel(){
    pixelIndex++;
    if(pixelIndex >= strip->numPixels()){
        pixelIndex = 0;
        return 1;
    }
    return 0;
}

void LED_Strip::nextMode(){
    mode++;
    if(mode >= numberOfModes) mode = 0;
}

void LED_Strip::setMode(int mode){
    if(mode >= numberOfModes) this->mode = 0;
    else this->mode = mode;
}

bool LED_Strip::repeat(int numberOfRepeat){
    repeatIndex++;
    if(repeatIndex >= numberOfRepeat){
        repeatIndex = 0;
        return 1;
    }
    return 0;
}

// Some functions of our own for creating animated effects -----------------
void LED_Strip::plainColor(uint32_t color, int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();
        for(int i=0; i<strip->numPixels(); i++) {
            strip->setPixelColor(i, color);         //  Set pixel's color (in RAM)
        }
        strip->show();

        if(repeat(numberOfRepeat)) nextMode();
    }
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void LED_Strip::colorWipe(uint32_t color, int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();
        strip->setPixelColor(pixelIndex, color);         //  Set pixel's color (in RAM)
        strip->show();                            //  Update strip to match

        if(nextPixel()){
            if(repeat(numberOfRepeat)) nextMode();
        }
    }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void LED_Strip::theaterChase(uint32_t color, int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();         //   Set all pixels in RAM to 0 (off)
        // 'c' counts up from 'b' to end of strip in steps of 3...
        for(int c=theaterChaseIndex; c<strip->numPixels(); c += 3) {
            strip->setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        strip->show(); // Update strip with new contents

        theaterChaseIndex++;
        if(theaterChaseIndex >= 3){
            theaterChaseIndex = 0;
            if(repeat(numberOfRepeat)) nextMode();
        }
    }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void LED_Strip::rainbow(int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        for(int i=0; i<strip->numPixels(); i++) {
            int pixelHue = rainbowFirstPixelHue + (i * 65536L / strip->numPixels());
            strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(pixelHue)));
        }
        strip->show();                            //  Update strip to match

        rainbowFirstPixelHue += 265;
        if(rainbowFirstPixelHue >= 65536){
            rainbowFirstPixelHue = 0;
            if(repeat(numberOfRepeat)) nextMode();
        }
    }

}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void LED_Strip::theaterChaseRainbow(int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();         //   Set all pixels in RAM to 0 (off)
        // 'c' counts up from 'b' to end of strip in steps of 3...
        for(int c=theaterChaseIndex; c<strip->numPixels(); c += 3) {
            // hue of pixel 'c' is offset by an amount to make one full
            // revolution of the color wheel (range 65536) along the length
            // of the strip (strip.numPixels() steps):
            int hue   = rainbowFirstPixelHue + c * 65536L / strip->numPixels();
            uint32_t color = strip->gamma32(strip->ColorHSV(hue)); // hue -> RGB
            strip->setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        strip->show(); // Update strip with new contents

        rainbowFirstPixelHue += 65536 / (numberOfRepeat * 3); // One cycle of color wheel over 90 frames

        theaterChaseIndex++;
        if(theaterChaseIndex >= 3){
            theaterChaseIndex = 0;
            if(repeat(numberOfRepeat)){
                nextMode();
                rainbowFirstPixelHue = 0;
            }
        }
    }
}



void LED_Strip::weldingWork(int numberOfRepeat) {
    if(doOnce){
        doOnce = false;

        strip->clear();
        for(int i=0; i<strip->numPixels(); i++) {
            strip->setPixelColor(i, strip->Color(255,   255,   255));         //  Set pixel's color (in RAM)
        }
        strip->show();

        weldingWorkTime = 1000;
    }

    long _currentTime = millis();
    if(_currentTime - _lastTime > weldingWorkTime) {
        _lastTime = _currentTime;
        weldingWorkTime = random(20,200);
        
        int rand = random(0,10);
        if(rand>5){
            int hue   = 65536L / 2 + random(0,65536L / 6);
            uint32_t color = strip->gamma32(strip->ColorHSV(hue)); // hue -> RGB
            strip->setPixelColor(3, color); // Set pixel 'c' to value 'color'
            strip->setPixelColor(4, color); // Set pixel 'c' to value 'color'
        }else if(rand>3){
            strip->setPixelColor(3, strip->Color(255,   255,   255));
            strip->setPixelColor(4, strip->Color(255,   255,   255));
        }else{
            strip->setPixelColor(3, strip->Color(0,   0,   0));
            strip->setPixelColor(4, strip->Color(0,   0,   0));
        }
        strip->show();

        if(repeat(numberOfRepeat)) doOnce = true;
    }
}