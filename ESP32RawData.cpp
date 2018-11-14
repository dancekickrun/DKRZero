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

#include "ESP32RawData.h"

ESP32RawData::ESP32RawData(JsonObject& json_process)
{

  ESP32Processor::fDataReady = true;
  accelerometer_enable=true;
  gyroscope_enable=true;
  magnetometer_enable=true;
  gps_enable=true;

  Setup(json_process);
};

ESP32RawData::~ESP32RawData(){};

// Initialize the processor
void ESP32RawData::Setup(JsonObject& json_process)
{
  fDelimiter = String(" ");

  if(json_process.containsKey("accelerometer")) accelerometer_enable = (bool) json_process["accelerometer"];
  if(json_process.containsKey("gyroscope")) gyroscope_enable = (bool) json_process["gyroscope"];
  if(json_process.containsKey("magnetometer")) magnetometer_enable = (bool) json_process["magnetometer"];
  if(json_process.containsKey("gps")) gps_enable = (bool) json_process["gps"];

  (accelerometer_enable) ? Serial.println("INFO: RawData accelerometer readout enabled") : Serial.println("INFO: RawData accelerometer readout disabled");
  (gyroscope_enable)? Serial.println("INFO: RawData gyroscope readout enabled") : Serial.println("INFO: RawData gyroscope readout disabled");
  (magnetometer_enable) ? Serial.println("INFO: RawData magnetometer readout enabled") : Serial.println("INFO: RawData magnetometer readout disabled");
  (gps_enable) ? Serial.println("INFO: RawData gps readout enabled") : Serial.println("INFO: RawData gps readout disabled");

  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back(String("time"));

  ESP32Processor::MessageTransportData.clear();


  if(accelerometer_enable)
  {
    ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_x"));
    ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_y"));
    ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_z"));
  }

  if(gyroscope_enable)
  {
    ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_x"));
    ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_y"));
    ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_z"));
  }

  if(magnetometer_enable)
  {
    ESP32Processor::MessageTransportNames.push_back(String("magnetometer_x"));
    ESP32Processor::MessageTransportNames.push_back(String("magnetometer_y"));
    ESP32Processor::MessageTransportNames.push_back(String("magnetometer_z"));
  }

  if(gps_enable)
  {
    ESP32Processor::MessageTransportNames.push_back(String("latitude"));
    ESP32Processor::MessageTransportNames.push_back(String("longitude"));
  }

  // ESP32Processor::MessageTransport = ESP32Processor::MessengerFormat::Space;





};


//
void ESP32RawData::ProcessData(long, float data){};
void ESP32RawData::ProcessData(long t, float lat, float lon,
  float aax, float aay, float aaz,
  float ggx, float ggy, float ggz,
  float mmx, float mmy, float mmz)
  {
    // Serial.println("ESP32RawData::ProcessData()");


    ESP32Processor::fCurrentTime = t;
    // ax=aax;ay=aay;az=aaz;
    // gx=ggx;gy=ggy;gz=ggz;
    // mx=mmx;my=mmy;mz=mmz;
    // fLatitude = lat;

    // ESP32Processor::MessageTransportData.clear();
    // ESP32Processor::MessageTransportData.shrink_to_fit();
    // fLongitude = lon;
    // MessageTransportData.reserve(768);

    ESP32Processor::MessageTransportData.clear();
    ESP32Processor::MessageTransportData.push_back((float) t);


    if(accelerometer_enable)
    {
      ESP32Processor::MessageTransportData.push_back(aax);
      ESP32Processor::MessageTransportData.push_back(aax);
      ESP32Processor::MessageTransportData.push_back(aax);
    }

    if(gyroscope_enable)
    {
      ESP32Processor::MessageTransportData.push_back(ggx);
      ESP32Processor::MessageTransportData.push_back(ggy);
      ESP32Processor::MessageTransportData.push_back(ggz);
    }

    if(magnetometer_enable)
    {
      ESP32Processor::MessageTransportData.push_back(mmx);
      ESP32Processor::MessageTransportData.push_back(mmy);
      ESP32Processor::MessageTransportData.push_back(mmz);
    }

    if(gps_enable)
    {
      ESP32Processor::MessageTransportData.push_back(lat);
      ESP32Processor::MessageTransportData.push_back(lon);
    }

  };
