// Copyright (c) 2018 by Chris Steer.
// All Rights Reserved.
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ESP32Magnetometer_hpp
#define ESP32Magnetometer_hpp

#include <vector>
#include <stdio.h>
#include <ArduinoJson.h>
#include "ESP32Sensor.h"



// Simple class to represent a sensor
class ESP32Magnetometer: public ESP32Sensor
{

public:

    ESP32Magnetometer(JsonObject& json_sensor);
    ~ESP32Magnetometer();

    void Setup(JsonObject& root);
    void ConfigureIMU(LSM9DS1* imu);

    bool GetLowPowerEnable(){return low_power_enable;};
    bool GetTemperatureCompensation(){return temperature_compensation_enable;};
    int GetXYPerformance(){return xy_performance;};
    int GetZPerformance(){return z_performance;};
    int GetOperationMode(){return operation_mode;};


private:

  bool low_power_enable;
  int xy_performance;
  int z_performance;
  int operation_mode;
  bool temperature_compensation_enable;

};


#endif /* ESP32Sensor_hpp */
