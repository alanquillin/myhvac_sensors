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

RgbLedColor RgbLedColor::getColorByName(String name){
  if(name.equalsIgnoreCase("red")){
    return RED;
  }
  if(name.equalsIgnoreCase("blue")){
    return BLUE;
  }
  if(name.equalsIgnoreCase("green")){
    return GREEN;
  }
  if(name.equalsIgnoreCase("yellow")){
    return YELLOW;
  }
  if(name.equalsIgnoreCase("white")){
    return WHITE;
  }

  return NONE;
}

RgbLedColor RgbLedColor::buildFromString(String str, char sep){
  int firstIndex = str.indexOf(sep);
  if(firstIndex == -1){
    Serial.print("Error parsing RBG.  Could not find any instances of separator '");
    Serial.print(sep);
    Serial.println("'");
    return NONE;
  }

  int secondIndex = str.indexOf(sep, firstIndex+1);
  if(secondIndex == -1){
    Serial.print("Error parsing RBG.  Could not find second instance of separator '");
    Serial.print(sep);
    Serial.println("'");
    return NONE;
  }

  int red = str.substring(0, firstIndex).toInt();
  int green = str.substring(firstIndex + 1, secondIndex).toInt();
  int blue = str.substring(secondIndex + 1).toInt();

  if(red < 0 || green < 0 || blue < 0){
    Serial.print("Error parsing RBG.  A non-int value found. Red: ");
    Serial.print(red);
    Serial.print(" - Green: ");
    Serial.print(green);
    Serial.print(" - Blue: ");
    Serial.print(blue);
    return NONE;
  }

  return RgbLedColor(red, green, blue, "CUSTOM");
}
