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


#include <vector>
#include <stdio.h>
#include "ESP32Gyroscope.h"

ESP32Gyroscope::ESP32Gyroscope(JsonObject& json_sensor)
{
  // Defaults
  enable=true;
  scale=2000;
  sample_rate=6;
  flip_x=false;
  flip_y=false;
  flip_z=false;

  high_resolution_enable=false;
  high_resolution_bandwidth=0;

  high_pass_filter_cutoff=0;
  high_pass_filter_disable=true;

  // Set the type
  ESP32Sensor::fSensorType = SensorType::Gyroscope;

  Setup(json_sensor);

}

ESP32Gyroscope::~ESP32Gyroscope()
{

}

void ESP32Gyroscope::Setup(JsonObject& json_sensor)
{


  if(json_sensor.containsKey("enable")) enable = (bool) json_sensor["enable"];
  if(json_sensor.containsKey("scale")) scale = atoi(json_sensor["scale"]);
  if(json_sensor.containsKey("sample_rate")) sample_rate = atoi(json_sensor["sample_rate"]);
  if(json_sensor.containsKey("low_power_enable")) low_power_enable = (bool) json_sensor["low_power_enable"];
  if(json_sensor.containsKey("high_resolution_bandwidth")) high_resolution_bandwidth = atoi(json_sensor["high_resolution_bandwidth"]);
  if(json_sensor.containsKey("high_pass_filter_cutoff")) high_pass_filter_cutoff = atoi(json_sensor["high_pass_filter_cutoff"]);
  // if(json_sensor.containsKey("bandwidth")) bandwidth = atoi(json_sensor["bandwidth"]);
  if(json_sensor.containsKey("flip_x")) flip_x = (bool) json_sensor["flip_x"];
  if(json_sensor.containsKey("flip_y")) flip_y = (bool) json_sensor["flip_y"];
  if(json_sensor.containsKey("flip_z")) flip_z = (bool) json_sensor["flip_z"];


  // Round the scale of the three values 245, 500
  if(scale<245) scale = 245;
  if((scale>245)&&(scale<372)) scale = 245;
  if((scale>372)&&(scale<500)) scale = 500;
  if(scale>500) scale = 2000;

  // Fix the sample rate
  if(sample_rate<1) sample_rate = 1;
  if(sample_rate>6) sample_rate = 6;

  // And the bandwidth
  // if(bandwidth<0) bandwidth = 0;
  // if(bandwidth>3) bandwidth = 3;

  // Fix the HPF cut off
  if(high_pass_filter_cutoff<0) high_pass_filter_cutoff = 0;
  if(high_pass_filter_cutoff>9) high_pass_filter_cutoff = 9;




}

void ESP32Gyroscope::ConfigureIMU(LSM9DS1* imu)
{

  (imu->settings).gyro.enabled = this->GetEnable();  // Enable the gyro
  (imu->settings).gyro.scale = this->GetScale(); // Set scale to +/-245dps
  (imu->settings).gyro.sampleRate = this->GetSampleRate(); // 59.5Hz ODR
  (imu->settings).gyro.lowPowerEnable = this->GetLowPowerEnable(); // LP mode off
  // (imu->settings).gyro.HPFEnable = true; // HPF disabled
  (imu->settings).gyro.HPFEnable = this->GetHighPassFilterDisable();
  (imu->settings).gyro.HPFCutoff = this->GetHighPassFilterCutoff(); // HPF cutoff = 4Hz
  (imu->settings).gyro.flipX = this->GetFlipX(); // Don't flip X
  (imu->settings).gyro.flipY = this->GetFlipY(); // Don't flip Y
  (imu->settings).gyro.flipZ = this->GetFlipZ(); // Don't flip Z

}
