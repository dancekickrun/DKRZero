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


#ifndef ESP32Accelerometer_hpp
#define ESP32Accelerometer_hpp

#include <vector>
#include <stdio.h>
#include <ArduinoJson.h>
#include "ESP32Sensor.h"


class ESP32Accelerometer : public ESP32Sensor
{

public:

    ESP32Accelerometer(JsonObject& root);
    ~ESP32Accelerometer();

    void Setup(JsonObject& root);
    void ConfigureIMU(LSM9DS1* imu);

    bool GetHighResEnable(){return high_resolution_enable;};
    int GetHighResBandwidth(){return high_resolution_bandwidth;};
    int GetBandwidth(){return bandwidth;};

private:

  bool high_resolution_enable;
  int high_resolution_bandwidth;
  int bandwidth;


};


#endif /* ESP32Sensor_hpp */
