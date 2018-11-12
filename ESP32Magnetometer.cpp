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
#include "ESP32Magnetometer.h"


ESP32Magnetometer::ESP32Magnetometer(JsonObject& json_sensor)
{
  // Defaults
  enable=true;
  scale=16;
  sample_rate=6;
  xy_performance=3;
  z_performance=3;
  low_power_enable=false;
  temperature_compensation_enable=true;
  operation_mode=0;

  // Set the type
  ESP32Sensor::fSensorType = SensorType::Magnetometer;

  Setup(json_sensor);

}

ESP32Magnetometer::~ESP32Magnetometer()
{

}

void ESP32Magnetometer::Setup(JsonObject& json_sensor)
{

// Just before calling this we should have had the lines
// Loop over all elements that are within the processors
// JsonArray& json_sensors = root["processors"];
// Then   for (auto& sensor : json_sensors) SensorFactory(sensor)

if(json_sensor.containsKey("enable")) enable = (bool) json_sensor["enable"];
if(json_sensor.containsKey("low_power_enable")) low_power_enable = (bool) json_sensor["low_power_enable"];

if(json_sensor.containsKey("xy_performance")) xy_performance = (bool) json_sensor["xy_performance"];
if(json_sensor.containsKey("z_performance")) z_performance = (bool) json_sensor["z_performance"];
if(json_sensor.containsKey("operation_mode")) operation_mode = atoi(json_sensor["operation_mode"]);

if(json_sensor.containsKey("sample_rate")) sample_rate = atoi(json_sensor["sample_rate"]);
if(json_sensor.containsKey("scale")) scale = atoi(json_sensor["scale"]);

if(json_sensor.containsKey("temperature_compensation_enable")) temperature_compensation_enable = (bool) json_sensor["temperature_compensation_enable"];


}

void ESP32Magnetometer::ConfigureIMU(LSM9DS1* imu)
{
  (imu->settings).mag.enabled = this->GetEnable(); // Enable magnetometer
  (imu->settings).mag.scale = this->GetScale(); // Set mag scale to +/-12 Gs
  (imu->settings).mag.sampleRate = this->GetSampleRate(); // Set OD rate to 20Hz
  (imu->settings).mag.tempCompensationEnable = this->GetTemperatureCompensation();
  (imu->settings).mag.XYPerformance = this->GetXYPerformance(); // Ultra-high perform.
  (imu->settings).mag.ZPerformance = this->GetZPerformance(); // Ultra-high perform.
  (imu->settings).mag.lowPowerEnable = this->GetLowPowerEnable();
  (imu->settings).mag.operatingMode = this->GetOperationMode(); // Continuous mode

}
