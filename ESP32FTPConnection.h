//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//
#ifndef ESP32FTPConnection_hpp
#define ESP32FTPConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>
#include <time.h>
#include <WiFi.h>
#include "ESP32Connection.h"
#include "ESP32Configuration.h"
#include "ESP32FtpServerSD.h"



// FTP connections class
// - should be subclassed from the WifiConnection class really
class ESP32FTPConnection : public ESP32Connection {

public:

    ESP32FTPConnection(const char* ssid, const char* password, int attempts);
    ~ESP32FTPConnection();

    void Initialize();
    void Start();
    void Stop();

    void Communicate(String);

    void GetTime();


    // FTP Server functions
    void StartFTPServer();
    void StopFTPServer();
    void CheckFTPServer();


private:

  // FTP Server variables
  FtpServer* fFTP;


  time_t fNetworkTime;

  int fAttempts;
  String fWifiIPAddress;
  const char*  fWifiNetworkName;
  const char*  fWifiNetworkPassword;


};


#endif
