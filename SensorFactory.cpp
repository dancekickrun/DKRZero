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


#include "SensorFactory.h"

ESP32Sensor* SensorFactory::ConstructSensor(JsonObject& json_sensor)
{
  Serial.println("INFO: Entering the Sensor Factory ");

  // Get the name of the sensor
  const char* name = json_sensor["name"];
  Serial.print("INFO: Creating sensor "); Serial.println(name);

  // And the options for the sensor name
  const char* accelerometer_name = "accelerometer";
  const char* gps_name = "gps";
  const char* gyroscope_name = "gyroscope";
  const char* magnetometer_name = "magnetometer";

  if (strcmp(name, accelerometer_name) == 0)
  {
    Serial.println("INFO: Setting up accelerometer");
    return new ESP32Accelerometer(json_sensor);
  }
  if (strcmp(name, gyroscope_name) == 0)
  {
    Serial.println("INFO: Setting up gyroscope");
    return new ESP32Gyroscope(json_sensor);
  }
  if (strcmp(name, magnetometer_name) == 0)
  {
    Serial.println("INFO: Setting up magnetometer");
    return new ESP32Magnetometer(json_sensor);
  }

}
