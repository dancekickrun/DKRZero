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

#ifndef ESP32RawData_hpp
#define ESP32RawData_hpp

#include <stdio.h>
#include "ESP32Processor.h"
#include <ArduinoJson.h>

class ESP32RawData : public ESP32Processor {
private:

public:

    ESP32RawData(JsonObject& json_process);
    ~ESP32RawData();

    // Initialize the processor
    void Setup(JsonObject& json_process);

    //
    void ProcessData(long, float data);
    void ProcessData(long, float , float, float, float, float, float, float, float, float, float, float);

    // float ax,ay,az,gx,gy,gz,mx,my,mz;
    // float fLatitude,fLongitude;
    // long fCurrentTime;



};


#endif /* ESP32RawData_hpp */
