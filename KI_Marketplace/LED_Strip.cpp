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
            break;
        case 1:
            setColor(255, 255, 255);
            plainColor(1000, 2); // White
            break;
        case 2:
            setColor(255,   0,   0);
            colorWipe(50, 2); // Red
            break;
        case 3:
            setColor(  0, 255,   0);
            colorWipe(50, 2); // Green
            break;
        case 4:
            setColor(   0,   0, 255);
            colorWipe(50, 2); // Blue
            break;
        case 5:
            rainbow(10, 5);             // Flowing rainbow cycle along the whole strip
            break;
        case 6:
            setColor(127,   0,   0);
            theaterChase(50, 10); // Red, half brightness
            break;
        case 7:
            setColor(127, 127, 127);
            theaterChase(50, 10); // White, half brightness
            break;
        case 8: 
            theaterChaseRainbow(50, 30); // Rainbow-enhanced theaterChase variant
            break;
        default:
            clear();
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

// set color of LED Strip -----------------
void LED_Strip::setColor(uint8_t r, uint8_t g, uint8_t b) {
    color.r = r;
    color.g = g;
    color.b = b;
}

void LED_Strip::setColor(COLOR color) {
    this->color = color;
}

void LED_Strip::clear() {
    strip->clear();
    strip->show();
}

// Some functions of our own for creating animated effects -----------------
bool LED_Strip::plainColor(int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();
        for(int i=0; i<strip->numPixels(); i++) {
            strip->setPixelColor(i, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
        }
        strip->show();

        if(repeat(numberOfRepeat)) return 1;
    }
    return 0;
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
bool LED_Strip::colorWipe(int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();
        strip->setPixelColor(pixelIndex, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
        strip->show();                            //  Update strip to match

        if(nextPixel()){
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
bool LED_Strip::theaterChase(int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();         //   Set all pixels in RAM to 0 (off)
        // 'c' counts up from 'b' to end of strip in steps of 3...
        for(int c=theaterChaseIndex; c<strip->numPixels(); c += 3) {
            strip->setPixelColor(c, strip->Color(color.r, color.g, color.b)); // Set pixel 'c' to value 'color'
        }
        strip->show(); // Update strip with new contents

        theaterChaseIndex++;
        if(theaterChaseIndex >= 3){
            theaterChaseIndex = 0;
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
bool LED_Strip::rainbow(int wait, int numberOfRepeat) {
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
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
bool LED_Strip::theaterChaseRainbow(int wait, int numberOfRepeat) {
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
                rainbowFirstPixelHue = 0;
                return 1;
            }
        }
    }
    return 0;
}