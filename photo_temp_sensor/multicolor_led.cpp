#include "multicolor_led.h"

MultiColorLED::MultiColorLED(){
    _enabled = true;
    _currentColor = OFF;


}

void MultiColorLED::init(){
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    setColor(OFF);

    Particle.function("enable_led", &MultiColorLED::cldEnable, this);
    Particle.function("disable_led", &MultiColorLED::cldDisable, this);
}

void MultiColorLED::setColor(LEDColorState color){
    _currentColor = color;

    if(_enabled || color.equals(OFF)){
        digitalWrite(LED_RED_PIN, color.getRed());
        digitalWrite(LED_GREEN_PIN, color.getGreen());
        digitalWrite(LED_BLUE_PIN, color.getBlue());
    }
}

int MultiColorLED::cldEnable(String data){
    enable();
    return 1;
}

int MultiColorLED::cldDisable(String data){
    disable();
    return 1;
}


void MultiColorLED::enable(){
    _enabled = true;

    setColor(_currentColor);
}

void MultiColorLED::disable(){
    _enabled = false;

    setColor(OFF);
}
