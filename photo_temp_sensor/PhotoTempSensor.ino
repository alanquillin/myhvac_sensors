#include "MCP9808.h"
#include "MeasurementService.h"
#include "RgbLed.h"
#include "RgbLedColor.h"
#include <math.h>

SYSTEM_MODE(MANUAL);

MeasurementService client;
MCP9808 mcp = MCP9808();
RgbLed led = RgbLed();
String myDeviceId;

bool is_registered = false;
bool is_server_available = false;
bool is_mcp_initialized = false;
int error_state_backoff_cnt = 1;
bool last_call_success = true;

double currentTempC = 0;
double last_temp_sent = 0;

int counter = 0;

void printTemp(float celcius, float fahrenheit)
{
  char str[100];
  sprintf(str, "Temperature: %f C, %f F", celcius, fahrenheit);
  Serial.println(str);
}

void check_cloud(){
  if(!Particle.connected()){
    Particle.connect();
  }
  Particle.process();
  Particle.disconnect();
}

void test_server(){
  is_server_available = client.Ping();
}

void register_sensor(){
  is_registered = client.IsSenorRegistered();
  if (is_registered == false){
    is_registered = client.RegisterSensor();
  }
}

void init_mcp(){
  is_mcp_initialized = mcp.begin();
  int mcp_init_cnt = 0;
  while(! is_mcp_initialized && mcp_init_cnt < 10){
    Serial.println("Main> MCP9808 not found");
    delay(1000);
    mcp_init_cnt = mcp_init_cnt + 1;
  }

  if(is_mcp_initialized == true){
    mcp.setResolution(MCP9808_SLOWEST);
  }
}


void send_temp_data(){
  if(! is_server_available || ! is_registered){
    return;
  }

  if(! last_call_success || round(last_temp_sent) != round(currentTempC)){
    led.setColor(BLUE);

    WiFi.on();
    WiFi.connect();
    waitUntil(WiFi.ready);

    double fahrenheit = c2f(currentTempC);
    if (! client.SendTemperatureData(currentTempC, fahrenheit)) {
      Serial.println("Main> Failed to send data to server. Testing if the server is available.");
      last_call_success = false;
      test_server();
    }
    else{
      last_temp_sent = currentTempC;
      last_call_success = true;
    }

    // Since the wifi is already on, lets check the cloud for anything.
    check_cloud();
    WiFi.off();
    led.setColor(GREEN);
  }
}

Timer sd_timer(120000, send_temp_data);


void setup()
{
    Serial.begin(9600);

    myDeviceId = Particle.deviceID();

    Serial.println("Main> Initializing... ");

    led.init();
    led.setColor(YELLOW);

    WiFi.on();
    WiFi.connect();
    waitUntil(WiFi.ready);
    check_cloud();

    Serial.print("Main> My device id: ");
    Serial.println(myDeviceId);

    client = MeasurementService(myDeviceId, "Photon", "Particle");
    test_server();
    if(is_server_available == true){
      register_sensor();
    }

    init_mcp();

    if(! is_registered || ! is_mcp_initialized || ! is_server_available){
      led.setColor(RED);
    }
    else{
      led.setColor(GREEN);
    }
    Serial.println("Main> Shutting down WiFi to save power...");
    WiFi.off();

    sd_timer.start();
    RGB.control(true);
    RGB.brightness(2);
    Serial.println("Main> Initialization complete.");
}

double c2f(double c){
  return c * 1.8 + 32.0;
}

void loop()
{
  if(! is_registered || ! is_mcp_initialized || ! is_server_available){
    led.setColor(RED);
    Serial.println("Main> The system is currently in an error state.  Sleeping..");
    if((error_state_backoff_cnt % 2) == 0){
      WiFi.on();
      WiFi.connect();
      waitUntil(WiFi.ready);

      check_cloud();
    }

    if(! is_server_available){
      WiFi.on();
      WiFi.connect();
      waitUntil(WiFi.ready);

      test_server();
    }

    if(is_server_available == true){
      WiFi.on();
      WiFi.connect();
      waitUntil(WiFi.ready);

      if(! is_registered){
        register_sensor();
      }
    }

    if(! is_mcp_initialized){
      init_mcp();
    }

    if(error_state_backoff_cnt < 10){
      error_state_backoff_cnt = error_state_backoff_cnt + 1;
    }
    // Make sure the wifi is off incase it was turned on
    // in one of the previous check above.
    WiFi.off();
    delay(10000 * error_state_backoff_cnt);
    return;
  }

  led.setColor(GREEN);
  error_state_backoff_cnt = 0;

  double celcius = 0.0;
  int cnt;
  while (celcius <= 0){
    celcius = mcp.getTemperature();
    cnt = cnt + 1;
    if (cnt >= 10){
      break;
    }
  }

  if(celcius > 0){
    currentTempC = celcius;
    double fahrenheit = c2f(celcius);
    printTemp(celcius, fahrenheit);
  }

  led.setColor(GREEN);
  delay(5000);
}
