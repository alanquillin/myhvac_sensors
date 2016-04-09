#ifndef __MEASUREMENT_SERVICE_H__
#define __MEASUREMENT_SERVICE_H__

#include "HttpClient.h"

class MeasurementService
{
public:
  MeasurementService();
  MeasurementService(String id, String model, String manufacturer);
  bool IsSenorRegistered();
  bool RegisterSensor();
  bool SendTemperatureData(double c, double f);
  bool Ping();

private:
  http_response_t Get(String path);
  http_response_t Post(String path, String data);

  String _db_id;
  String _id;
  String _model;
  String _manufacturer;
};

#endif
