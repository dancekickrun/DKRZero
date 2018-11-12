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

  Setup(json_process);
};

ESP32RawData::~ESP32RawData(){};

// Initialize the processor
void ESP32RawData::Setup(JsonObject& json_process)
{
  fDelimiter = String(" ");

  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back(String("time"));
  ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_x"));
  ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_y"));
  ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_z"));

  ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_x"));
  ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_y"));
  ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_z"));

  ESP32Processor::MessageTransportNames.push_back(String("magnetometer_x"));
  ESP32Processor::MessageTransportNames.push_back(String("magnetometer_y"));
  ESP32Processor::MessageTransportNames.push_back(String("magnetometer_z"));

  ESP32Processor::MessageTransportNames.push_back(String("latitude"));
  ESP32Processor::MessageTransportNames.push_back(String("longitude"));

  // ESP32Processor::MessageTransport = ESP32Processor::MessengerFormat::Space;

  ESP32Processor::MessageTransportData.clear();
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
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

    ESP32Processor::MessageTransportData[0] = (float) t;

    ESP32Processor::MessageTransportData[1] =  aax;
    ESP32Processor::MessageTransportData[2] =  aay;
    ESP32Processor::MessageTransportData[3] =  aaz;

    ESP32Processor::MessageTransportData[4] =  ggx;
    ESP32Processor::MessageTransportData[5] =  ggy;
    ESP32Processor::MessageTransportData[6] =  ggz;

    ESP32Processor::MessageTransportData[7] =  mmx;
    ESP32Processor::MessageTransportData[8] =  mmy;
    ESP32Processor::MessageTransportData[9] =  mmz;

    ESP32Processor::MessageTransportData[10] =  lat;
    ESP32Processor::MessageTransportData[11] =  lon;

  };
