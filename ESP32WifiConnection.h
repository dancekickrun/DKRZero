//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//
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
