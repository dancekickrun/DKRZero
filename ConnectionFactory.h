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

#ifndef ConnectionFactory_hpp
#define ConnectionFactory_hpp

#include <vector>
#include <stdio.h>
#include <ArduinoJson.h>

// Connections' declarations
#include "ESP32Connection.h"
// =
#include "ESP32BLEConnection.h"
#include "ESP32WifiConnection.h"
// #include "ESP32FTPConnection.h"
#include "ESP32SDCardConnection.h"


namespace ConnectionFactory {

ESP32Connection* ConstructConnection(JsonObject& json_sensor);

}// end of ConnectionFactory namespace

#endif
