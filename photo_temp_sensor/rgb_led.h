#ifndef RGB_LED_H
#define RGB_LED_H

#include "application.h"
#include "rgb_led_color.h"

#define LED_BLUE_PIN    D2
#define LED_GREEN_PIN   D3
#define LED_RED_PIN     A3

class RgbLed
{
public:
  enum LedType
  {
    COMMON_ANODE,
    COMMON_CATHODE
  };

  enum LedWriteStyle
  {
    ANALOG,
    DIGITAL
  };

  enum LedBrightness
  {
    B_HIGH,
    B_MEDIUM_HIGH,
    B_MEDIUM,
    B_MEDIUM_LOW,
    B_LOW,
    B_VERY_LOW,
    B_EXTREMELY_LOW
  };

  RgbLed();
  RgbLed(LedType type, LedBrightness brightness, LedWriteStyle led_write_style);

  void init();
  void setColor(RgbLedColor color);
  void enable();
  void disable();
  int cldEnable(String data);
  int cldSetBrightness(String data);

private:
  void _setup(LedType type, LedBrightness brightness, LedWriteStyle led_write_style);
  void _setColor(int pin, int val);
  void _writePinValue(int pin, int val);

  RgbLedColor _currentColor;
  bool _enabled;
  LedType _led_type;
  LedBrightness _brightness;
  LedWriteStyle _led_write_style;
  double _off, _elow_mult, _vlow_mult, _low_mult, _mlow_mult, _med_mult, _mhigh_mult, _high_mult;
  PinState _d_high, _d_low;
};

#endif
