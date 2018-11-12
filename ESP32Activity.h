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

#ifndef ESP32Activity_hpp
#define ESP32Activity_hpp

#include <vector>
#include <stdio.h>
#include "ESP32Processor.h"
#include <ArduinoJson.h>


class ESP32Activity : public ESP32Processor {
public:
    ESP32Activity(JsonObject& json_process);
    ~ESP32Activity();

    // Initialize the processor
    void Setup(JsonObject&  json_process);

    //
    void ProcessData(long, float data);
    void ProcessData(long t,float la, float lo,
      float ax,float ay,float az,
      float gx,float gy,float gz,
      float mx,float my,float mz);

    void SetLowThreshold(float thresh){fLowThreshold = thresh;};
    void SetMidThreshold(float thresh){fMidThreshold = thresh;};
    void SetHighThreshold(float thresh){fHighThreshold = thresh;};

private:

  float fLowThreshold;
  float fMidThreshold;
  float fHighThreshold;

};


#endif /* ESP32Cadence_hpp */
