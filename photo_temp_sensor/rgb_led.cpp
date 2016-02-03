#include "rgb_led.h"

RgbLed::RgbLed(){
  _setup(COMMON_ANODE, B_EXTREMELY_LOW, ANALOG);
}

RgbLed::RgbLed(LedType led_type, LedBrightness brightness, LedWriteStyle led_write_style){
  _setup(led_type, brightness, led_write_style);
}

void RgbLed::_setup(LedType led_type, LedBrightness brightness, LedWriteStyle led_write_style){
  Serial.println("Setting up LED");
  _enabled = true;
  _currentColor = OFF;
  _brightness = brightness;
  _led_type = led_type;
  _led_write_style = led_write_style;
}

void RgbLed::init(){
  switch(_led_type){
    case COMMON_CATHODE:
      _off = 0;
      _elow_mult = .02;
      _vlow_mult = .16;
      _low_mult = .33;
      _mlow_mult = .5;
      _med_mult = .66;
      _mhigh_mult = .84;
      _high_mult = 1;
      _d_high = HIGH;
      _d_low = LOW;
      break;
    case COMMON_ANODE:
      _off = RgbColorMax;
      _elow_mult = .98;
      _vlow_mult = .84;
      _low_mult = .66;
      _mlow_mult = .5;
      _med_mult = .33;
      _mhigh_mult = .16;
      _high_mult = 0;
      _d_high = LOW;
      _d_low = HIGH;
      break;
  }

  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  setColor(OFF);

  Particle.function("enable_led", &RgbLed::cldEnable, this);
  Particle.function("set_bright", &RgbLed::cldSetBrightness, this);
}

void RgbLed::setColor(RgbLedColor color){
  _currentColor = color;

  if(_enabled){
    Serial.print("Setting Blue pin value: ");
    _setColor(LED_BLUE_PIN, color.getBlue());
      Serial.print("Setting Green pin value: ");
    _setColor(LED_GREEN_PIN, color.getGreen());
      Serial.print("Setting Red pin value: ");
    _setColor(LED_RED_PIN, color.getRed());
  }
}


void RgbLed::_setColor(int pin, int val){
  _writePinValue(pin, val);
}

void RgbLed::_writePinValue(int pin, int val){
  if(_led_write_style == ANALOG){
    double value = _off;
    if(val > 0){
      switch(_brightness){
        case B_HIGH:
          value = val * _high_mult;
          break;
        case B_MEDIUM_HIGH:
          value = val * _mhigh_mult;
          break;
        case B_MEDIUM:
          value = val * _med_mult;
          break;
        case B_MEDIUM_LOW:
          value = val * _mlow_mult;
          break;
        case B_LOW:
          value = val * _low_mult;
          break;
        case B_EXTREMELY_LOW:
          value = val * _elow_mult;
          break;
        case B_VERY_LOW:
          value = val * _vlow_mult;
          break;
      }
    }
    Serial.println(value);
    analogWrite(pin, value);
  }
  else{
    digitalWrite(pin, val > 0 ? _d_high : _d_low);
  }
}

int RgbLed::cldEnable(String data){
    if(data.equalsIgnoreCase(String("true"))){
      enable();
      return 1;
    }

    if(data.equalsIgnoreCase(String("false"))){
      disable();
      return 1;
    }

    return 0;
}

int RgbLed::cldSetBrightness(String data){
  if(data.equalsIgnoreCase(String("high")) || data.equalsIgnoreCase(String("h"))){
    _brightness = B_HIGH;
  }
  else if(data.equalsIgnoreCase(String("medium high")) || data.equalsIgnoreCase(String("mhigh")) || data.equalsIgnoreCase(String("mh"))){
    _brightness = B_MEDIUM_HIGH;
  }
  else if(data.equalsIgnoreCase(String("medium")) || data.equalsIgnoreCase(String("med")) || data.equalsIgnoreCase(String("m"))){
    _brightness = B_MEDIUM;
  }
  else if(data.equalsIgnoreCase(String("medium low")) || data.equalsIgnoreCase(String("mlow")) || data.equalsIgnoreCase(String("ml"))){
    _brightness = B_MEDIUM_LOW;
  }
  else if(data.equalsIgnoreCase(String("low")) || data.equalsIgnoreCase(String("l"))){
    _brightness = B_LOW;
  }
  else if(data.equalsIgnoreCase(String("very low")) || data.equalsIgnoreCase(String("vlow")) || data.equalsIgnoreCase(String("vl"))){
    _brightness = B_VERY_LOW;
  }
  else if(data.equalsIgnoreCase(String("extremely low")) || data.equalsIgnoreCase(String("elow")) || data.equalsIgnoreCase(String("el"))){
    _brightness = B_EXTREMELY_LOW;
  }
  else{
    return -1;
  }

  setColor(_currentColor);
}

void RgbLed::enable(){
  Serial.println("Enabling the LED");
  _enabled = true;
  setColor(_currentColor);
}

void RgbLed::disable(){
  Serial.println("Disabling the LED");
  setColor(OFF);
  _enabled = false;
}
