#include "MCP9808.h"
#include "MeasurementService.h"
#include "RgbLed.h"
#include "RgbLedColor.h"
#include <math.h>

MeasurementService client;
MCP9808 mcp = MCP9808();
RgbLed led = RgbLed();
String myDeviceId;

bool is_registered = false;
bool is_server_available = false;
bool is_mcp_initialized = false;
int error_state_backoff_cnt = 1;

double currentTempC = 0;
int counter = 0;

int LOOP_DELAY_MS = 5000; // 2.5 sec
int FORCE_SEND_TEMP_INTERVAL_MS = 300000; // 5 min

int TEMP_LOOP_CNT_THRESHOLD;

void printTemp(float celcius, float fahrenheit)
{
  Serial.printlnf("Main> Temperature: %f C, %f F", celcius, fahrenheit);
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

void setup()
{
    Serial.begin(9600);

    myDeviceId = Particle.deviceID();

    Serial.println("Main> Initializing... ");

    // Calculate the number of times we chack to see if the temp has changed
    // before we forcefully send the data to the server even if the temp
    // has not changed.
    TEMP_LOOP_CNT_THRESHOLD = (FORCE_SEND_TEMP_INTERVAL_MS / LOOP_DELAY_MS);

    Serial.printlnf("Main> Loop delay %f (S), %d (MS).", (LOOP_DELAY_MS / 1000), LOOP_DELAY_MS);
    Serial.printlnf("Main> Time delay before send %f (S), %d (MS)", (FORCE_SEND_TEMP_INTERVAL_MS / 1000), FORCE_SEND_TEMP_INTERVAL_MS);
    Serial.printlnf("Main> Check temp counter threshold (MAX counter value): %d", TEMP_LOOP_CNT_THRESHOLD);

    led.init();
    led.disable();
    led.setColor(YELLOW);

    RGB.control(true);
    RGB.brightness(0);
    RGB.control(false);

    Serial.print("Main> My device id: ");
    Serial.println(myDeviceId);

    client = MeasurementService(myDeviceId, "Photon", "Particle");
    test_server();
    if(is_server_available == true){
      register_sensor();
    }
    else{
      Serial.println("Main> Service is currently unavailable.");
    }

    Serial.println("Main> Attempting to initialize the temperature sensor");
    init_mcp();
    if(! is_mcp_initialized){
      Serial.println("Main> Temperature sensor Initialization failed");
    }

    if(! is_registered || ! is_mcp_initialized || ! is_server_available){
      led.setColor(RED);
    }
    else{
      led.setColor(GREEN);
    }

    Serial.println("Main> Initialization complete.");

    Serial.flush();
}

double c2f(double c){
  return c * 1.8 + 32.0;
}

double abs_d(double x){
  if (x < 0) return x * -1;

  return x;
}

void loop()
{
  if(! is_registered || ! is_mcp_initialized || ! is_server_available){
    led.setColor(RED);
    Serial.println("Main> The system is currently in an error state.  Sleeping..");

    delay(10000 * error_state_backoff_cnt);

    if(! is_server_available){
      test_server();
    }

    if(is_server_available == true){
      if(! is_registered){
        register_sensor();
      }
    }
    else{
      Serial.println("Main> Service is currently unavailable.");
    }

    if(! is_mcp_initialized){
      Serial.println("Main> Attempting to initialize the temperature sensor");
      init_mcp();
      if(! is_mcp_initialized){
        Serial.println("Main> Temperature sensor Initialization failed");
      }
    }

    return;
  }

  led.setColor(GREEN);
  error_state_backoff_cnt = 1;

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
    double fahrenheit = c2f(celcius);
    printTemp(celcius, fahrenheit);


    // If the temp has not changed (+/- .5 degrees) no need to send the data
    // However, if the counter reaches 10, then send the data anyway
    // to notify the server that you are still alive :)
    double diff = currentTempC - celcius;
    if (abs_d(diff) > .5 || counter > TEMP_LOOP_CNT_THRESHOLD) {
      led.setColor(BLUE);

      Serial.print("Main> Sending current temp data to server.");
      Serial.printlnf("Previous temp: %f C, Current temp: %f C", currentTempC, celcius);

      currentTempC = celcius;
      if (! client.SendTemperatureData(celcius, fahrenheit)) {
        Serial.println("Main> Failed to send data to server, will try again next time");
      }
      else {
        counter = 0;
      }

      // If it has failed 5 times to send the data, then assume the service is
      // down and put the system in an error state
      if(counter > TEMP_LOOP_CNT_THRESHOLD + 5){
        Serial.println("Main> Failed to send data to the server several times.  Putting system in error state.");
        is_server_available = false;
      }
    }
    else{
      counter = counter + 1;
    }
  }

  led.setColor(GREEN);
  Serial.flush();
  delay(LOOP_DELAY_MS);
}
