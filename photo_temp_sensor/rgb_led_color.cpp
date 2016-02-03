#include "rgb_led_color.h"

RgbLedColor::RgbLedColor(){
  _setup(RgbColorOff, RgbColorOff, RgbColorOff, String("UNKNOWN"));
}

RgbLedColor::RgbLedColor(int red, int green, int blue, String name){
    _setup(red, green, blue, name);
}

void RgbLedColor::_setup(int red, int green, int blue, String name){
  _red = red;
  _green = green;
  _blue = blue;
  _name = name;
}

int RgbLedColor::getRed(){
  return _red;
}

int RgbLedColor::getGreen(){
  return _green;
}

int RgbLedColor::getBlue(){
  return _blue;
}

String RgbLedColor::getName(){
  return _name;
}

bool RgbLedColor::equals(RgbLedColor color){
  return _red == color.getRed() && _green == color.getGreen() && _blue == color.getBlue() && _name.equals(color.getName());
}
