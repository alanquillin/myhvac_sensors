#ifndef MULTICOLOR_LED_H
#define MULTICOLOR_LED_H

#include "application.h"
#include "led_color_state.h"

#define LED_BLUE_PIN    D2
#define LED_GREEN_PIN   D3
#define LED_RED_PIN     D4

class MultiColorLED
{
public:
    MultiColorLED();

    void init();
    void setColor(LEDColorState color);
    void enable();
    void disable();
    int cldEnable(String data);
    int cldDisable(String data);

private:
    LEDColorState _currentColor;
    bool _enabled;
};

#endif
