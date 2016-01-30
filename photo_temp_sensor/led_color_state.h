#ifndef LED_COLOR_STATE_H
#define LED_COLOR_STATE_H

#include "application.h"

class LEDColorState
{
public:
    LEDColorState();
    LEDColorState(PinState red, PinState green, PinState blue);

    PinState getRed();
    PinState getGreen();
    PinState getBlue();
    bool equals(LEDColorState color);

private:
    PinState _red, _green, _blue;
};

#define RED     LEDColorState(LOW, HIGH, HIGH)
#define GREEN   LEDColorState(HIGH, LOW, HIGH)
#define BLUE    LEDColorState(HIGH, HIGH, LOW)
#define YELLOW  LEDColorState(LOW, LOW, HIGH)
#define OFF     LEDColorState(HIGH, HIGH, HIGH)

#endif
