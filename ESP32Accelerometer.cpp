// Copyright (c) 2018 by Chris Steer->
// All Rights Reserved->
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version->
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE->  See the
// GNU General Public License for more details->
//
// You should have received a copy of the GNU General Public License
// along with this program->  If not, see <http://www->gnu->org/licenses/>->

#include <vector>
#include <stdio.h>
#include "ESP32Accelerometer.h"


ESP32Accelerometer::ESP32Accelerometer(JsonObject& json_sensor)
{
  // Defaults
  enable=true;
  enable_x=true;
  enable_y=true;
  enable_z=true;
  scale=16;
  sample_rate=6;
  bandwidth=-1;
  high_resolution_enable=false;
  high_resolution_bandwidth=0;

  // Set the type
  ESP32Sensor::fSensorType = SensorType::Accelerometer;


  Setup(json_sensor);
}



ESP32Accelerometer::~ESP32Accelerometer()
{

}

void ESP32Accelerometer::Setup(JsonObject& json_sensor)
{

  if(json_sensor.containsKey("enable")) enable = (bool) json_sensor["enable"];
  if(json_sensor.containsKey("enable_x")) enable_x = (bool) json_sensor["enable_x"];
  if(json_sensor.containsKey("enable_y")) enable_y = (bool) json_sensor["enable_y"];
  if(json_sensor.containsKey("enable_z")) enable_z = (bool) json_sensor["enable_z"];

  if(json_sensor.containsKey("bandwidth")) bandwidth = atoi(json_sensor["bandwidth"]);
  if(json_sensor.containsKey("scale")) scale = atoi(json_sensor["scale"]);
  if(json_sensor.containsKey("sample_rate")) sample_rate = atoi(json_sensor["sample_rate"]);

  if(json_sensor.containsKey("high_resolution_enable")) high_resolution_enable = (bool) json_sensor["high_resolution_enable"];
  if(json_sensor.containsKey("bandwidth")) bandwidth = atoi(json_sensor["bandwidth"]);
  if(json_sensor.containsKey("high_resolution_bandwidth")) high_resolution_bandwidth = (bool) json_sensor["high_resolution_bandwidth"];

  // If not enabled then disable all axes
  if(!enable){
    enable_x = false;
    enable_y = false;
    enable_z = false;
  }

  // Fix the scale of the accelerometer
  if(scale>16) scale = 16;
  if(scale<1) scale = 1;

  // Bound the sample rate
  if(sample_rate>6) sample_rate = 6;
  if(sample_rate<1) sample_rate = 1;

  // The scale should be a multiple of 2
  if(scale<3){
    scale = 2;
  } else if (scale<6) {
    scale = 4;
  } else if (scale<12) {
    scale = 8;
  } else {
    scale = 16;
  }

  // And bound the bandwidth input
  if(bandwidth<-1) bandwidth = -1;
  if(bandwidth>3) bandwidth = 3;

  // Bound the high res bandwidth
  if(high_resolution_bandwidth>3) high_resolution_bandwidth = 3;
  if(high_resolution_bandwidth<0) high_resolution_bandwidth = 0;


  // Print the configuration
  Serial.print("INFO: imu.settings.accel.sampleRate = "); Serial.println(this->GetSampleRate());
  Serial.print("INFO: imu.settings.accel.bandwidth = "); Serial.println(this->GetBandwidth());
  Serial.print("INFO: imu.settings.accel.scale = "); Serial.println(this->GetScale());
  Serial.print("INFO: imu.settings.accel.enable = "); Serial.println(this->GetEnable());
  Serial.print("INFO: imu.settings.accel.enableX = "); Serial.println(this->GetEnableX());
  Serial.print("INFO: imu.settings.accel.enableY = "); Serial.println(this->GetEnableY());
  Serial.print("INFO: imu.settings.accel.enableZ = "); Serial.println(this->GetEnableZ());
  Serial.print("INFO: imu.settings.accel.highResEnable = "); Serial.println(this->GetHighResEnable());
  Serial.print("INFO: imu.settings.accel.highResBandwidth = "); Serial.println(this->GetHighResBandwidth());

}


void ESP32Accelerometer::ConfigureIMU(LSM9DS1* imu)
{
(imu->settings).accel.sampleRate = this->GetSampleRate();
(imu->settings).accel.bandwidth = this->GetBandwidth();
(imu->settings).accel.scale = this->GetScale();
(imu->settings).accel.enabled = this->GetEnable();
(imu->settings).accel.enableX = this->GetEnableX(); // Enable X
(imu->settings).accel.enableY = this->GetEnableY();// Enable Y
(imu->settings).accel.enableZ = this->GetEnableZ();// Enable Z
(imu->settings).accel.highResEnable = this->GetHighResEnable(); // Disable HR
(imu->settings).accel.highResBandwidth = this->GetHighResBandwidth();

}
