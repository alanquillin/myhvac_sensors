#include "rgb_led.h"
#include "rgb_led_color.h"
#include "MCP9808.h"

TCPClient client;
MCP9808 mcp = MCP9808();
RgbLed led = RgbLed();
String myDeviceId;

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

    myDeviceId = Particle.deviceID();

    Serial.print("My device id: ");

    Serial.println(myDeviceId);

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
    led.setColor(BLUE);
    delay(1000);
    led.setColor(GREEN);
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
  double celcius = 0.0;
  int cnt;
  while (celcius <= 0){
    celcius = mcp.getTemperature();
    cnt = cnt + 1;
    if (cnt >= 10){
      break;
    }
  }

  double fahrenheit = c2f(celcius);
  printTemp(celcius, fahrenheit);

  currentTempC = celcius;
  currentTempF = fahrenheit;
  Serial.flush();

  delay(5000);
}
