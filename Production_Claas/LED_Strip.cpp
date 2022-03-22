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

// turn off the strip
void LED_Strip::clear() {
    strip->clear();
    strip->show();
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

// set color of LED Strip (COLOR)
void LED_Strip::setColor(COLOR color) {
    this->color = color;
}

// return number of LEDs of the strip
int LED_Strip::getNumberOfLEDs(){
    return strip->numPixels();
}


// Fill all strip pixels with one plain color
// Pass delay time (in ms) between frames.
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
        }

        strip->setPixelColor(pixelIndex, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
        strip->show();                            //  Update strip to match

        if(nextPixel(direction, startPixel, endPixel)){
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

// Fill strip pixels one by one with a color beginning at the startPixel
// and moving in the direction until the endPixel is reached. 
// Only one Pixel shows the color at each time. 
// Pass delay time (in ms) between frames.
bool LED_Strip::colorWipeOneByOne_doubleStrip(int wait, bool direction, int startPixel, int endPixel, int numberOfRepeat) {
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        //Initial Condition
        if(setStartPixel){
            setStartPixel = false;
            pixelIndex = startPixel;
        }

        strip->clear();
        strip->setPixelColor(pixelIndex, strip->Color(color.r, color.g, color.b));         //  Set pixel's color (in RAM)
        strip->setPixelColor(pixelIndex + strip->numPixels()/2, strip->Color(color.r, color.g, color.b));
        strip->show();                            //  Update strip to match

        if(nextPixel(direction, startPixel, endPixel)){
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}

bool LED_Strip::dataWipe(int wait, const bool datastream[], int datastreamLength, int numberOfRepeat){
    long _currentTime = millis();
    if(_currentTime - _lastTime > wait) {
        _lastTime = _currentTime;

        //Initial Condition
        if(setStartPixel){
            setStartPixel = false;
            dataIndex = 0;
        }

        strip->clear();
        for(int i = 0; i < strip->numPixels(); i++){
            int index = dataIndex - i;
            if(index >= 0 && index < datastreamLength){
                if(datastream[index]){
                    strip->setPixelColor(i, strip->Color(color.r, color.g, color.b));
                } 
            }
        }
        strip->show();

        dataIndex++;
        if(dataIndex >= datastreamLength + strip->numPixels()){
            setStartPixel = true;
            if(repeat(numberOfRepeat)) return 1;
        }
    }
    return 0;
}