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

#ifndef ProcessFactory_hpp
#define ProcessFactory_hpp

#include <vector>
#include <stdio.h>
#include <ArduinoJson.h>

#include "ESP32Processor.h"

// Processors' declarations
#include "ESP32Activity.h"
#include "ESP32Cadence.h"
#include "ESP32RawData.h"
#include "ESP32Impulse.h"
#include "ESP32SensorFusion.h" //SF
#include "ESP32SampleFrequency.h"
#include "ESP32SampleStatistics.h"
// #include "ESP32Geolocation.h"

namespace ProcessFactory {

ESP32Processor* ConstructProcess(JsonObject& json_sensor);

}// end of ProcessFactory namespace

#endif
