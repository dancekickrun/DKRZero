//
//  ESP32Configuration.hpp
//  trigger
//
//  Created by Chris Steer on 09/08/2018.
//
//

#ifndef ESP32Configuration_h
#define ESP32Configuration_h



#include "FileSerial.h"
#include <SparkFunLSM9DS1.h>
#include <gp20u7.h>
#include <stdio.h>
#include "ESP32FtpServerSD.h"
#include <ArduinoJson.h>
#include <vector>

#include "ESP32Processor.h"

// This is a singleton class which needs to be accesible globally
class ESP32Configuration {

public:

  // Should also make copy and destructor private
  ESP32Configuration();

  void Configure(JsonObject& root);

  void SetupSDCard();
  // void FTPSetup();
  void SetupSensors(JsonObject &root);
  void SetupWriteData(JsonObject &root);
  void SetupGyroscope(JsonObject &root);
  void SetupAccelerometer(JsonObject &root);
  void SetupMagnetometer(JsonObject &root);
  void SetupTemperature(JsonObject &root);
  // void SetupWifi(JsonObject &root);
  // void SetupMQTT(JsonObject &root);

  void SetupGPS(JsonObject &root);
  void Print();

  // void ConnectWifi();
  // void DisconnectWifi();

  bool GetTemperatureEnable(){return fTemperatureEnable;};
  bool GetGyroscopeEnable(){return fGyroEnable;};
  bool GetGPSEnable(){return fGPSEnable;};
  bool GetMagnetometerEnable(){return fMagEnable;};
  bool GetAccelerometerEnable(int axis)
  {
    return fAccelXEnable || fAccelYEnable || fAccelZEnable;
  }

  void Indicator(int nflash, int led_on_ms, int led_off_ms,int led_end_pause);

  // FileSerial* GetDataFile(){return fDataLogFile;};
  // FileSerial* GetInfoFile(){return fInfoLogFile;};

  // FtpServer* GetFTPServer(){return ftpSrv;};
  LSM9DS1* GetIMU(){return (&imu);};
  GP20U7* GetGPS(){return (&gps);};
  HardwareSerial* GetGPSSerial(){return (&Serial1); };

  // // WiFiClient* GetMQTTWifiClient(){return (&fMQTTClient);};
  // PubSubClient* GetPubSubClient(){return (&fPubSubClient);};

  // String GetWiFiIPAddress(){return fWifiIPAddress; };
  float GetSampleRate(){return fDesiredDataRate;};

  // bool GetWifiEnable(){return fWifiEnable;};
  bool GetMQTTEnable(){return fMQTTEnable;};

private:

  //
  // WiFiClient fMQTTClient;
  // PubSubClient fPubSubClient;

  // FtpServer *ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
  float fDesiredDataRate;

  GP20U7 gps;
  LSM9DS1 imu;  // Create an LSM9DS1 object

  HardwareSerial Serial1;  // UART1/Serial1 pins 16,17

  /**
   * Logging related
   */

   // MQTT
   bool fMQTTEnable;
   const char* fMQTTServerIP;
   int fMQTTServerPort;
   const char* fMQTTThingName;
   const char* fMQTTAccessToken;

  // Write data to the SD Card
  FileSerial* fDataLogFile;
  FileSerial* fInfoLogFile;
  bool fSaveInfoLog;
  bool fSerialOutputEnable;
  bool fDashboardEnable;
  bool fFileWriteEnable;
  bool fSilentRunning;

  /**
   *  GPS
   */
     bool fGPSEnable;

  /**
  * Accelerometer variables
  */
  bool fAccelEnable;
  bool fAccelXEnable;
  bool fAccelYEnable;
  bool fAccelZEnable;
  bool fAccelHighResEnable;

  int fAccelScale;
  int fAccelBandwidth;
  int fAccelSampleRate;
  int fAccelHighResBandwidth;


  /**
  * Gyro variables
  */
  bool fGyroEnable;
  bool fGyroLowPowerEnable;
  bool fGyroFlipX;
  bool fGyroFlipY;
  bool fGyroFlipZ;

  int fGyroHPFDisable;
  int fGyroSampleRate;
  int fGyroScale;
  int fGyroHPFCutOff;
  int fGyroBandwidth;


  /**
  * Magnetometer Variables
  */
  bool fMagLowPowerEnable;
  bool fMagEnable;
  bool fMagTempCompEnable;
  int fMagScale;
  int fMagXYPerformance;
  int fMagSampleRate;
  int fMagZPerformance;
  int fMagOperationMode;

  /**
  * Temperature Variables
  */
  bool fTemperatureEnable;

  /**
  * Wifi Related
  */
  // bool fWifiEnable;
  // const char*  fWifiNetworkName;
  // const char*  fWifiNetworkPassword;
  // int fWifiStatus;
  // String fWifiIPAddress;



};

#endif /* ESP32Configuration_hpp */
