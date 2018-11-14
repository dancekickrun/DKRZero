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

#ifndef ESP32uWave_hpp
#define ESP32uWave_hpp

#include <vector>
#include <stdio.h>
#include "ESP32Processor.h"
#include <ArduinoJson.h>
#include "Arduino.h"
#include <SD.h>



class ESP32uWave : public ESP32Processor {

public:

  ESP32uWave(JsonObject& json_process);
  ~ESP32uWave();

  // Initialize the processor
  void Setup(JsonObject&  json_process);

  //
  void ProcessData(long, float data);
  void ProcessData(long t,float la, float lo,
    float ax,float ay,float az,
    float gx,float gy,float gz,
    float mx,float my,float mz);

  private:

    void ReadAccelerometerData(JsonObject& uWave_template);
    void ReadGyroscopeData(JsonObject& uWave_template);
    void ReadMagnetometerData(JsonObject& uWave_template);

    // The dictionary filename
    char* fDictionaryFile;

    // The template names of the dictionary
    std::vector<String> fTemplateNames;
    std::vector< std::vector<int> > fAccelerationTemplates;
    std::vector< std::vector<int> > fGyroscopeTemplates;
    std::vector< std::vector<int> > fMagnetometerTemplates;


  };


  #endif /*  */
