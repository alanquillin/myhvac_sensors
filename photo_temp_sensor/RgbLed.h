#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include "application.h"
#include "RgbLedColor.h"

#define LED_BLUE_PIN    A4
#define LED_GREEN_PIN   A5
#define LED_RED_PIN     A7

class RgbLed
{
public:
  struct Cmd
  {
    String key;
    String value;
  };

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
  int cldExecuteLedCommand(String data);

private:
  void _setup(LedType type, LedBrightness brightness, LedWriteStyle led_write_style);
  void _setColor(int pin, int val);
  void _setBrightness(String value);
  void _writePinValue(int pin, int val);
  void _parseCommands(String data, Cmd* commands, int i);
  void _parseCommand(String command, Cmd* commands, int i);

  RgbLedColor _currentColor;
  bool _enabled;
  LedType _led_type;
  LedBrightness _brightness;
  LedWriteStyle _led_write_style;
  double _off, _elow_mult, _vlow_mult, _low_mult, _mlow_mult, _med_mult, _mhigh_mult, _high_mult;
  PinState _d_high, _d_low;
};

#endif
