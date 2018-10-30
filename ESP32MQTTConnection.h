//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//
#ifndef ESP32MQTTConnection_hpp
#define ESP32MQTTConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>
#include <time.h>
#include <WiFi.h>
#include "ESP32Connection.h"

#include <PubSubClient.h>

// Wifi connections class
class ESP32MQTTConnection : public ESP32Connection
{

public:

    ESP32MQTTConnection(
      const char* wifi_ssid,
      const char* wifi_password,
      int wifi_attempts,
      const char* mqtt_server,
      const int mqtt_port,
      const char* mqtt_thing_name,
      const char* mqtt_thing_access_token
    );
    //   fMQTTServerIP = (const char*) root["mqtt_server"];
    //   fMQTTServerPort = (const int) root["mqtt_port"];// 1883
    //   fMQTTThingName = (const char*) root["mqtt_thing_name"];
    //
    //
    //   // const char* esp_thing_name = "ESP-001";
    //   fMQTTAccessToken = (const char*) root["mqtt_thing_access_token"];


    ~ESP32MQTTConnection();

    void Initialize();
    void Start();
    void Stop();

    void GetTime();
    void Communicate(String);

private:

  WiFiClient fPubSubWifiClient;
  PubSubClient fPubSubClient;

  time_t fNetworkTime;

  int fAttempts;


  String fWifiIPAddress;
  const char*  fWifiNetworkName;
  const char*  fWifiNetworkPassword;

  const char* fMQTTServerIP;
  const int fMQTTServerPort;// 1883
  const char* fMQTTThingName;
  const char* fMQTTAccessToken;



};
#endif
