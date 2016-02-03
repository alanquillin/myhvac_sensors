#include "rgb_led.h"

/****************************************************
********** Constructors and Initialization **********
*****************************************************/

RgbLed::RgbLed(){
  _setup(COMMON_ANODE, B_MEDIUM, ANALOG);
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


/************************************************
********** Public Functions *********************
*************************************************/

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

  Particle.function("led_command", &RgbLed::cldExecuteLedCommand, this);
}

void RgbLed::setColor(RgbLedColor color){
  _currentColor = color;

  if(_enabled){
    //Serial.print("Setting Blue pin value: ");
    _setColor(LED_BLUE_PIN, color.getBlue());
    //Serial.print("Setting Green pin value: ");
    _setColor(LED_GREEN_PIN, color.getGreen());
    //Serial.print("Setting Red pin value: ");
    _setColor(LED_RED_PIN, color.getRed());
  }
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

/************************************************
********** Cloud Enabled Functions **************
*************************************************/

int RgbLed::cldExecuteLedCommand(String data){
  Cmd * commands = new Cmd[21];
  _parseCommands(data, commands, 0);

  for(int i = 0; i < 21; i++){
    Cmd cmd = commands[i];

    if(cmd.key.length() > 0) {
      Serial.print("Processing command: ");
      Serial.print(cmd.key);
      Serial.print("=");
      Serial.println(cmd.value);
      if(cmd.key.equalsIgnoreCase("enabled")) {
        if(cmd.value.equalsIgnoreCase(String("true"))) {
          enable();
        }

        if(cmd.value.equalsIgnoreCase(String("false"))) {
          disable();
        }
      }
      else if(cmd.key.equalsIgnoreCase("brightness")){
        _setBrightness(cmd.value);
      }
      else {
        Serial.print("Unknown LED command recieved: ");
        Serial.print(cmd.key);
        Serial.println("... ignoring.");
      }
    }
  }
}

/************************************************
********** Private Functions ********************
*************************************************/

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
    //Serial.println(value);
    analogWrite(pin, value);
  }
  else{
    digitalWrite(pin, val > 0 ? _d_high : _d_low);
  }
}

void RgbLed::_parseCommands(String data, Cmd* commands, int i){
  Serial.print("Parsing LED commands.  Data: ");
  Serial.println(data);
  int index = data.indexOf(",", 0);
  Serial.print("Index for first '=' ");
  Serial.println(index);
  if(index == -1){
    _parseCommand(data, commands, i);
    return;
  }

  _parseCommand(data.substring(0, index), commands, i);
  _parseCommands(data.substring(index + 1), commands, i+1);
}

void RgbLed::_parseCommand(String command, Cmd* commands, int i){
  Serial.print("Parsing LED command.  Command: ");
  Serial.println(command);
  int index = command.indexOf("=", 0);
  if(index == -1){
    return;
  }

  struct Cmd cmd;
  cmd.key = command.substring(0, index);
  cmd.value = command.substring(index + 1);
  commands[i] = cmd;
}

void RgbLed::_setBrightness(String data){
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
    Serial.print("Unknown brightness command value: ");
    Serial.println(data);
  }

  setColor(_currentColor);
}
