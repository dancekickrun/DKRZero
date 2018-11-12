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

#include "ESP32SampleFrequency.h"

ESP32SampleFrequency::ESP32SampleFrequency(JsonObject& json_process)
{
  fNPoints=20;

  Setup(json_process);
}

ESP32SampleFrequency::~ESP32SampleFrequency()
{

}

void ESP32SampleFrequency::Setup(JsonObject& json_process)
{
  Serial.println("INFO: Setting up ESP32SampleFrequency ");
  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back("sample_frequency");

  if(json_process.containsKey("num_points")) fNPoints = atoi(json_process["num_points"]);

  for(int i=0;i<fNPoints;i++)
  {
    tmp_data.push_back((long) 0);
  }
};



void ESP32SampleFrequency::ProcessData(long t,float la, float lo,
  float ax,float ay,float az,
  float gx,float gy,float gz,
  float mx,float my,float mz)
  {
    ESP32Processor::MessageTransportData.clear();
    ESP32Processor::MessageTransportData.push_back(t);

    // remove the first element
    tmp_data.erase(tmp_data.begin());
    // add to the last element
    tmp_data.push_back(t);

    // Calculate the average time difference
    fDiffTime=0;
    for(int j=0;j<tmp_data.size()-1;j++) fDiffTime += (float) (tmp_data.at(j+1)-tmp_data.at(j));
    fDiffTime /= ((float) (tmp_data.size()-1));

    ESP32Processor::MessageTransportData.push_back(((float) 1000.0)/((float) fDiffTime));

  };

void ESP32SampleFrequency::ProcessData(long CurrentTime, float data_point){};
