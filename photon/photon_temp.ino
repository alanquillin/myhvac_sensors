#include "OneWire/OneWire.h"
#include "spark-dallas-temperature/spark-dallas-temperature.h"

DallasTemperature temp_sensor_d(new OneWire(D3));
TCPClient client;

void print_temp(char* type, float celcius, float fahrenheit)
{
  Serial.print("[");
  Serial.print(type);
  Serial.print("] Temperature in: ");
  Serial.print("Celcius= ");
  Serial.print(celcius);
  Serial.print(", Fahrenheit= ");
  Serial.println(fahrenheit);
}

void send_data()
{

}

void setup()
{
  Serial.begin(9600);

  temp_sensor_d.begin();
}

void loop()
{
  temp_sensor_d.requestTemperatures();
  float temp = temp_sensor_d.getTempCByIndex(0);
  float celcius = 0.0;
  float fahrenheit = temp*9.0/5.0 + 32.0;

  print_temp("DIGITAL", celcius, fahrenheit);

  temp = analogRead(A0);
  fahrenheit = (((temp * 3.3) / 4095) - .5) * 100

  print_temp("ANALOG", celcius, fahrenheit);

  Serial.flush();
  delay(2000);
}
