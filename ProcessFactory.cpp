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

#include "ProcessFactory.h"

ESP32Processor* ProcessFactory::ConstructProcess(JsonObject& json_sensor)
{

  // Get the name of the sensor
  const char* name = json_sensor["name"];

  Serial.print("INFO: Creating processor "); Serial.println(name);

  const char* cadence_process_name = "cadence";
  if (strcmp(name, cadence_process_name) == 0)
  {
    Serial.println("INFO: Created cadence processor");
    return new ESP32Cadence(json_sensor);
  }

  const char* sf_process_name = "sensorfusion";
  if (strcmp(name, sf_process_name) == 0)
  {
    Serial.println("INFO: Created sensor fusion processor");
    return new ESP32SensorFusion(json_sensor);
  }

  const char* raw_process_name = "raw";
  if (strcmp(name, raw_process_name) == 0)
  {
    Serial.println("INFO: Created raw processor");
    return new ESP32RawData(json_sensor);
  }

  const char* impulse_process_name = "impulse";
  if (strcmp(name, impulse_process_name) == 0)
  {
    Serial.println("INFO: Created impulse processor");
    return new ESP32Impulse(json_sensor);
  }

  const char* activity_process_name = "activity";
  if (strcmp(name, activity_process_name) == 0)
  {
    Serial.println("INFO: Created activity processor");
    return new ESP32Activity(json_sensor);
  }

  const char* sample_freq_process_name = "sample_frequency";
  if (strcmp(name, sample_freq_process_name) == 0)
  {
    Serial.println("INFO: Created sample frequency processor");
    return new ESP32SampleFrequency(json_sensor);
  }

  const char* sample_stats_process_name = "sample_statistics";
  if (strcmp(name, sample_stats_process_name) == 0)
  {
    Serial.println("INFO: Created sample statistics processor");
    return new ESP32SampleStatistics(json_sensor);
  }

}
