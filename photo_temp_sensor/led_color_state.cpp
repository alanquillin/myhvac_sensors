#include "led_color_state.h"

LEDColorState::LEDColorState(){
    _red = HIGH;
    _green = HIGH;
    _blue = HIGH;
}

LEDColorState::LEDColorState(PinState red, PinState green, PinState blue){
    _red = red;
    _green = green;
    _blue = blue;
}

PinState LEDColorState::getRed(){
    return _red;
}

PinState LEDColorState::getGreen(){
    return _green;
}

PinState LEDColorState::getBlue(){
    return _blue;
}

bool LEDColorState::equals(LEDColorState color){
    return _red == color.getRed() && _green == color.getGreen() && _blue == color.getBlue();
}
