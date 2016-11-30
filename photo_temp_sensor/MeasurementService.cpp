#include "application.h"
#include "HttpClient.h"
#include "MeasurementService.h"

String HOST = "192.168.1.11";
int PORT = 8080;
http_header_t GET_HEADERS[] = {
    { "Accept" , "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_header_t POST_HEADERS[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

HttpClient http_client;

/****************************************************
********** Constructors and Initialization **********
*****************************************************/
MeasurementService::MeasurementService(){
  _db_id = NULL;
  _id = NULL;
  _model = NULL;
  _manufacturer = NULL;
}

MeasurementService::MeasurementService(String id, String model, String manufacturer){
  _db_id = NULL;
  _id = id;
  _model = model;
  _manufacturer = manufacturer;
}

/************************************************
********** Public Functions *********************
*************************************************/
bool MeasurementService::Ping(){
  String path = "/system/ping";

  int cnt = 0;
  while(cnt < 5){
    http_response_t response = Get(path);
    if (response.status == 200){
      return true;
    }
    cnt = cnt + 1;
  }
  return false;
}

bool MeasurementService::IsSenorRegistered(){
  String path = "/sensors/";
  if(_db_id == NULL){
    path.concat(_id);
  }
  else{
    path.concat(_db_id);
  }

  int cnt = 0;
  while(cnt < 5){
    http_response_t response = Get(path);
    if (response.status == 200){
      return true;
    }

    if (response.status == 404){
      return false;
    }
    cnt = cnt + 1;
  }
  return false;
}

bool MeasurementService::RegisterSensor(){
  String path = "/sensors";

  String data = "{\"sensor\": {\"name\":\"";
  data.concat(_id);
  data.concat("\", \"manufacturer_id\": \"");
  data.concat(_id);
  data.concat("\", \"type\": {\"model\": \"");
  data.concat(_model);
  data.concat("\", \"manufacturer\": \"");
  data.concat(_manufacturer);
  data.concat("\"}}}");

  int cnt = 0;
  while(cnt < 5){
    http_response_t response = Post(path, data);
    if (response.status == 201){
      return true;
    }
    cnt = cnt + 1;
  }
  return false;
}

bool MeasurementService::SendTemperatureData(double c, double f){
  String path = "/sensors/";
  if(_db_id == NULL){
    path.concat(_id);
  }
  else{
    path.concat(_db_id);
  }
  path.concat("/measurements/temperatures");

  String data = "{\"temperature\":{\"f\": ";
  data.concat(f);
  data.concat(", \"c\": ");
  data.concat(c);
  data.concat("}}");

  int cnt = 0;
  while(cnt < 5){
    http_response_t response = Post(path, data);
    if (response.status == 201){
      return true;
    }
    cnt = cnt + 1;
  }
  return false;
}

/************************************************
********** Private Functions ********************
*************************************************/

http_response_t MeasurementService::Get(String path){
  http_request_t request;
  request.hostname = HOST;
  request.port = PORT;
  request.path = path;

  http_response_t response;

  Serial.print("MeasurementService>\tGET http://");
  Serial.print(HOST);
  Serial.print(":");
  Serial.print(PORT);
  Serial.println(path);

  http_client.get(request, response, GET_HEADERS);

  Serial.print("MeasurementService>\tResponse status: ");
  Serial.println(response.status);
  Serial.print("MeasurementService>\tHTTP Response Body: ");
  Serial.println(response.body);
  return response;
}

http_response_t MeasurementService::Post(String path, String data){
  http_request_t request;
  request.hostname = HOST;
  request.port = PORT;
  request.path = path;
  request.body = data;

  http_response_t response;

  Serial.print("MeasurementService>\tPOST http://");
  Serial.print(HOST);
  Serial.print(":");
  Serial.print(PORT);
  Serial.println(path);
  Serial.print("MeasurementService>\tData: ");
  Serial.println(data);

  http_client.post(request, response, POST_HEADERS);

  Serial.print("MeasurementService>\tResponse status: ");
  Serial.println(response.status);
  Serial.print("MeasurementService>\tHTTP Response Body: ");
  Serial.println(response.body);
  return response;
}
