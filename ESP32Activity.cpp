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


#include "ESP32Activity.h"

ESP32Activity::ESP32Activity(JsonObject& json_process) 
{

  fLowThreshold = 1.2;
  fMidThreshold = 2.0;
  fHighThreshold = 5.0;


  Setup(json_process);

}

ESP32Activity::~ESP32Activity() {}


void ESP32Activity::Setup(JsonObject& json_process){

  if(json_process.containsKey("low_activity")) fLowThreshold = atof(json_process["low_activity"]);
  if(json_process.containsKey("mid_activity")) fMidThreshold = atof(json_process["mid_activity"]);
  if(json_process.containsKey("high_activity")) fHighThreshold = atof(json_process["high_activity"]);

  ESP32Processor::MessageTransportNames.push_back(String("low_activity"));
  ESP32Processor::MessageTransportNames.push_back(String("mid_activity"));
  ESP32Processor::MessageTransportNames.push_back(String("high_activity"));

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

};

void ESP32Activity::ProcessData(long t,float la, float lo,
  float ax,float ay,float az,
  float gx,float gy,float gz,
  float mx,float my,float mz)
  {
    // Serial.println("ESP32Cadence::ProcessData()");
    ProcessData(t, sqrt(ax*ax+ay*ay+az*az));
  };

//
void ESP32Activity::ProcessData(long CurrentTime, float data_point)
{
  data_point -=1.0;
  data_point = abs(data_point);

  bool low_activity = (data_point>fLowThreshold)&&(data_point<fMidThreshold);
  bool mid_activity = (data_point>fMidThreshold)&&(data_point<fHighThreshold);
  bool hi_activity = (data_point>fHighThreshold);

  ESP32Processor::MessageTransportData[0] = low_activity ? 1.0 : 0.0;
  ESP32Processor::MessageTransportData[1] = mid_activity ? 1.0 : 0.0;
  ESP32Processor::MessageTransportData[2] = hi_activity ? 1.0 : 0.0;

};
