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

#ifndef ESP32SerialConnection_hpp
#define ESP32SerialConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>
#include "ESP32Connection.h"
#include <ArduinoJson.h>

// Serial connections class
class ESP32SerialConnection : public ESP32Connection {

public:

    ESP32SerialConnection(JsonObject& json_connection);

    ~ESP32SerialConnection();

    void Start(){};
    void Stop(){};
    void Initialize(){};

    void Setup(JsonObject& json_connection);

    void Communicate(String);


private:

  String output_data;
  long fNumWrites;
  int fWriteModulo;

};


#endif
