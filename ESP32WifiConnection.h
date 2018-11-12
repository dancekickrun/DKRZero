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

#ifndef ESP32WifiConnection_hpp
#define ESP32WifiConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>
#include <time.h>
#include <WiFi.h>
#include "ESP32Connection.h"



// Wifi connections class
class ESP32WifiConnection : public ESP32Connection
{

public:

    ESP32WifiConnection(const char* ssid, const char* password, int attempts);
    ~ESP32WifiConnection();

    void Initialize();
    void Start();
    void Stop();

    void GetTime();
    void Communicate(String);

private:


  time_t fNetworkTime;

  int fAttempts;
  String fWifiIPAddress;
  const char*  fWifiNetworkName;
  const char*  fWifiNetworkPassword;

};
#endif
