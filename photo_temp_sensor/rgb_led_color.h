#ifndef RGB_LED_COLOR_H
#define RGB_LED_COLOR_H

#include "application.h"

#define RgbColorMax 255
#define RgbColorOff 0

class RgbLedColor
{
public:
  RgbLedColor();
  RgbLedColor(int red, int green, int blue, String name);

  int getRed();
  int getGreen();
  int getBlue();
  String getName();
  bool equals(RgbLedColor color);

  static RgbLedColor getColorByName(String name);
  static RgbLedColor buildFromString(String str, char sep = ':');

private:
  void _setup(int red, int green, int blue, String name);
  int _red, _green, _blue;
  String _name;
};

#define RED     RgbLedColor(RgbColorMax, RgbColorOff, RgbColorOff, "Red")
#define GREEN   RgbLedColor(RgbColorOff, RgbColorMax, RgbColorOff, "Green")
#define BLUE    RgbLedColor(RgbColorOff, RgbColorOff, RgbColorMax, "Blue")
#define YELLOW  RgbLedColor(RgbColorMax, RgbColorMax, RgbColorOff, "Yellow")
#define WHITE   RgbLedColor(RgbColorMax, RgbColorMax, RgbColorMax, "White")
#define OFF     RgbLedColor(RgbColorOff, RgbColorOff, RgbColorOff, "Off")
#define NONE     RgbLedColor(RgbColorOff, RgbColorOff, RgbColorOff, "Off")

#endif
