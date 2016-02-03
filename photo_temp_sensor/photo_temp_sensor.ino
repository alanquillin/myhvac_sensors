#include "rgb_led.h"
#include "rgb_led_color.h"
#include "MCP9808.h"

TCPClient client;
MCP9808 mcp = MCP9808();
RgbLed led = RgbLed();

double currentTempC = 0;
double currentTempF = 0;

void printTemp(float celcius, float fahrenheit)
{
    char str[100];
    sprintf(str, "Temperature: %f C, %f F", celcius, fahrenheit);
    Serial.println(str);
}

void setup()
{
    Serial.begin(9600);

    led.init();
    led.setColor(YELLOW);

    Serial.println("Initializing... ");

    while(! mcp.begin()){
        led.setColor(RED);
        Serial.println("MCP9808 not found");
        delay(1000);
    }

    mcp.setResolution(MCP9808_SLOWEST);

    Particle.variable("currentTempC", &currentTempC, DOUBLE);
    Particle.variable("currentTempF", &currentTempF, DOUBLE);

    delay(2000);
    led.setColor(RED);
    delay(1000);
    Serial.println("Initialization complete.");
}

void send_data()
{

}

double c2f(double c){
    return c * 1.8 + 32.0;
}

void loop()
{
    led.setColor(BLUE);
    double celcius = mcp.getTemperature();
    double fahrenheit = c2f(celcius);
    printTemp(celcius, fahrenheit);

    currentTempC = celcius;
    currentTempF = fahrenheit;
    Serial.flush();

    led.setColor(GREEN);

    delay(5000);
}
