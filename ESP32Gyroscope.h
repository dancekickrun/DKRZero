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

#ifndef ESP32Gyroscope_hpp
#define ESP32Gyroscope_hpp

#include <vector>
#include <stdio.h>
#include <ArduinoJson.h>
#include "ESP32Sensor.h"


// Simple class to represent a sensor
class ESP32Gyroscope: public ESP32Sensor
{

public:

    ESP32Gyroscope(JsonObject& json_sensor);
    ~ESP32Gyroscope();

    void Setup(JsonObject& json_sensor);
    void ConfigureIMU(LSM9DS1* imu);


    bool GetHighResEnable(){return high_resolution_enable;};
    bool GetLowPowerEnable(){return low_power_enable;};
    int GetHighResBandwidth(){return high_resolution_bandwidth;};

    int GetHighPassFilterCutoff(){return high_pass_filter_cutoff;};
    bool GetHighPassFilterDisable(){return high_pass_filter_disable;};

    bool GetFlipX(){return flip_x;};
    bool GetFlipY(){return flip_y;};
    bool GetFlipZ(){return flip_z;};

private:

  bool low_power_enable;
  bool high_resolution_enable;
  int high_resolution_bandwidth;
  int high_pass_filter_cutoff;
  bool high_pass_filter_disable;
  bool flip_x;
  bool flip_y;
  bool flip_z;

};


#endif /* ESP32Gyroscope_hpp */
