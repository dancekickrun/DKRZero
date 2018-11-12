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

#ifndef ESP32SDCardConnection_hpp
#define ESP32SDCardConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>
#include "ESP32Connection.h"
#include "FileSerial.h"
#include <ArduinoJson.h>

// SD Card connections class - assumes that the SD card is already mounted to read the config.txt file
class ESP32SDCardConnection : public ESP32Connection {

public:

    ESP32SDCardConnection(JsonObject& json_connection);

    ~ESP32SDCardConnection();

    void Setup(JsonObject& json_connection);

    void Initialize();
    void Start();
    void Stop();
    void Communicate(String);


private:

  FileSerial* fDataLogFile;
  const char* fStubFilename;
  bool fContinuousAcq;// if true then don't get a new file for every event
  int fMaxFileSize;
  int fWriteBufferSize;

};


#endif
