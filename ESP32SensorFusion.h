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

#ifndef ESP32SensorFusion_hpp
#define ESP32SensorFusion_hpp

#include "ESP32Processor.h"
#include "ESP32SensorFusion.h" //SF
#include "SensorFusion.h" //SF
#include <stdio.h>
#include <ArduinoJson.h>

class ESP32SensorFusion : public ESP32Processor {

public:

    //
    ESP32SensorFusion(JsonObject& json_process);
    ~ESP32SensorFusion();

    // Initialize the processor
    void Setup(JsonObject& json_process);

    String Message();

    //
    void ProcessData(long, float la, float lo, float ax,float ay,float az, float gx,float gy,float gz, float mx,float my,float mz);

protected:

  bool fMahony;
  bool fMagEnable;
  float fPitch, fRoll, fYaw;

  // For the Sensor Fusion
  SF filter;

  float deltat;

 private:

};


#endif /* SensorFusion_hpp */
