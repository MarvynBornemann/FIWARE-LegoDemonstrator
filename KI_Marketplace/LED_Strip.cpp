#include "LED_Strip.h"

LED_Strip::LED_Strip(int numberOfLEDs, int pinLED){
    strip = new Adafruit_NeoPixel(numberOfLEDs, pinLED, NEO_GRB + NEO_KHZ800);
    _lastTime = millis();
    pixelIndex = 0;
}

LED_Strip::~LED_Strip(){
    delete strip;
}

void LED_Strip::setup(){
    strip->begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip->show();            // Turn OFF all pixels ASAP
    strip->setBrightness(LED_BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
}

//Select next Pixel depending on the direction.
//Return 1 if the next Pixel reached the endPixel or the startPixel
bool LED_Strip::nextPixel(bool direction, int startPixel, int endPixel){
    //next pixel index depending on direction
    if(direction) pixelIndex--;
    else pixelIndex++;

    //reset pixel index if end of strip is reached
    if(pixelIndex >= strip->numPixels()){
        pixelIndex = 0;
    }
    if(pixelIndex < 0){
        pixelIndex = strip->numPixels() - 1;
    }

    //next pixel color
    chooseColorByIndex(pixelIndex);

    //return 1 if startPixel or EndPixel is reached
    int _endPixel = -1;
    if(endPixel > -1){
        if(direction) _endPixel = endPixel - 1;
        else _endPixel = endPixel + 1;
        if(_endPixel >= strip->numPixels()){
            _endPixel = 0;
        }
        if(_endPixel < 0){
            _endPixel = strip->numPixels() - 1;
        }
    }
    if(pixelIndex == startPixel || pixelIndex == _endPixel){
        setStartPixel = true;
        return 1;
    }
    return 0;
}

//return 1 if this function got executed #numberOfRepeat times
bool LED_Strip::repeat(int numberOfRepeat){
    repeatIndex++;
    if(repeatIndex >= numberOfRepeat){
        repeatIndex = 0;
        return 1;
    }
    return 0;
}

// set color of LED Strip (r,g,b)
void LED_Strip::setColor(uint8_t r, uint8_t g, uint8_t b) {
    color.r = r;
    color.g = g;
    color.b = b;
}

// set color of LED Strip (COLOR)
void LED_Strip::setColor(COLOR color) {
    this->colorMain = color;
    colorSecondIsSet = false;
}

// set second color of LED Strip (COLOR) for alternating colors at colorWipes
void LED_Strip::setColorSecond(COLOR color) {
    this->colorSecond = color;
    colorSecondIsSet = true;
}

// alternate colors if second coor is set
void LED_Strip::chooseColorByIndex(int index) {
    color = colorMain;
    if(colorSecondIsSet){
        if(index % 2 == 1){
            color = colorSecond;
        }
    }
}

// return number of LEDs of the strip
int LED_Strip::getNumberOfLEDs(){
    return strip->numPixels();
}

// turn off the strip
void LED_Strip::clear() {
    strip->clear();
    strip->show();
}

// Fill all strip pixels with one plain color
// Pass delay time (in ms) between frames.
bool LED_Strip::plainColor(int wait, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        strip->clear();
        for(int i=0; i<strip->numPixels(); i++) {
            strip->setPixelColor(i, strip->Color(colorMain.r, colorMain.g, colorMain.b));         //  Set pixel's color (in RAM)
        }
        strip->show();

        if(repeat(numberOfRepeat)) return 1;
    }
    return 0;
}


// Fill strip pixels one after another with a color beginning at the startPixel
// and moving in the direction until the endPixel is reached.
// At the end all pixels from startPixel to endPixel show the same color.
// Pass delay time (in ms) between frames.
bool LED_Strip::colorWipe(int wait, bool direction, int startPixel, int endPixel, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        //Initial Condition
        if(setStartPixel){
            setStartPixel = false;
            pixelIndex = startPixel;
            chooseColorByIndex(startPixel);
        }

        strip->setPixelColor(pixelIndex, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
        strip->show();                            //  Update strip to match

        if(nextPixel(direction, startPixel, endPixel)){
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}


// Fill strip pixels one after another with a color beginning at the startPixel
// and moving in both directions until both endPixel1 and endPixel2 are reached.
// At the end all pixels in between endPixel1 and endPixel2 show the same color.
// Pass delay time (in ms) between frames.
bool LED_Strip::colorWipeDouble(int wait, int startPixel, int endPixel1, int endPixel2, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        //Initial Condition
        if(setStartPixel){
            setStartPixel = false;
            pixelIndex = 0;
            chooseColorByIndex(pixelIndex);
        }

        //ToDo: is needed because strips connect to ring at pixels 1,3,5,..
        chooseColorByIndex(pixelIndex + 1);

        //calculate next Pixels
        int startPixelPlus = (startPixel + pixelIndex) % strip->numPixels();
        int startPixelMinus = (startPixel - pixelIndex);
        if(startPixelMinus < 0) startPixelMinus = strip->numPixels() + startPixelMinus;

        if(!colorWipeDoubleFinished1){
            strip->setPixelColor(startPixelPlus, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
            //check if one endPixel is reached
            if(startPixelPlus == endPixel1 || startPixelPlus == endPixel2) colorWipeDoubleFinished1 = true;
        } 
        if(!colorWipeDoubleFinished2){
            strip->setPixelColor(startPixelMinus, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
            //check if one endPixel is reached
            if(startPixelMinus == endPixel1 || startPixelMinus == endPixel2) colorWipeDoubleFinished2 = true;
        } 
        strip->show();                            //  Update strip to match

        if(nextPixel() || (colorWipeDoubleFinished1 && colorWipeDoubleFinished2)){
            colorWipeDoubleFinished1 = false;
            colorWipeDoubleFinished2 = false;
            setStartPixel = true;
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}

// Fill strip pixels one by one with a color beginning at the startPixel
// and moving in the direction until the endPixel is reached. 
// Only one Pixel shows the color at each time. 
// Pass delay time (in ms) between frames.
bool LED_Strip::colorWipeOneByOne(int wait, bool direction, int startPixel, int endPixel, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        //Initial Condition
        if(setStartPixel){
            setStartPixel = false;
            pixelIndex = startPixel;
            chooseColorByIndex(startPixel);
        }

        strip->clear();
        strip->setPixelColor(pixelIndex, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
        strip->show();                            //  Update strip to match

        if(nextPixel(direction, startPixel, endPixel)){
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}

// Theater-marquee-style chasing lights. Pass delay time (in ms) between frames.
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

// Rainbow cycle plain color with whole strip. Pass delay time (in ms) between frames.
bool LED_Strip::rainbowPlainColor(int wait, int startPixel, int numberOfPixels, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        int pixelHue = rainbowFirstPixelHue + (startPixel * 65536L / strip->numPixels());
        if(numberOfPixels > 0){
            pixelHue = rainbowFirstPixelHue + (startPixel * 65536L / numberOfPixels);
        }
        
        for(int i=0; i<strip->numPixels(); i++) {
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