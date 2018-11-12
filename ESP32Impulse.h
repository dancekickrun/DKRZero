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

#ifndef ESP32Impulse_h
#define ESP32Impulse_h

#include <stdio.h>
#include "ESP32Processor.h"
#include <math.h>
#include <ArduinoJson.h>
#include <queue>

class ESP32Impulse : public ESP32Processor
{

public:


  ESP32Impulse(JsonObject& json_process);


  ~ESP32Impulse();


  // Initialize the processor
  void Setup(JsonObject& json_process);

  // void CopyDataToMessage();

  void ProcessData(long t,
    float lat,float lon,
    float a, float b,float c,
    float d, float e,float f,
    float g, float h, float i);


  std::vector<bool> fOnOffs;

  // The data buffer
  std::vector< std::vector<float> > fMeasurements;

  int fThresholdCoord;
  int fBufferSize;
  float fBufferThreshold;
  int fBufferDelayCount;
  int fBufferDelay;
  bool fBufferDelayOn;

  bool fNoNames;

  int fDataCount;

  };


#endif /* ESP32Impulse_hpp */
