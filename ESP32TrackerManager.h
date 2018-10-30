#ifndef ESP32Tracker_h
#define ESP32Tracker_h

#include "Arduino.h"

#include <Esp.h>
#include <vector>
#include<ArduinoJson.h>
#include <gp20u7.h>

// Processors' declarations
#include "ESP32Cadence.h"
#include "ESP32RawData.h"
#include "ESP32Impulse.h"
#include "ESP32SensorFusion.h" //SF
#include "ESP32SampleFrequency.h"
#include "ESP32SampleStatistics.h"
// #include "ESP32Geolocation.h"

#include "SPIFFS.h"
// #include "ESP8266FtpServer.h"

// Connections' declarations
#include "ESP32Connection.h"
#include "ESP32BLEConnection.h"
#include "ESP32WifiConnection.h"
#include "ESP32FTPConnection.h"
#include "ESP32SDCardConnection.h"

#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

enum Error{ ConfigReadFail, IMUCommsFail, ProcessUnrecognized};


class ESP32TrackerManager
{

public:


  ESP32TrackerManager();

  // Configuration Start Functions
  void Configure();
  void StartIMU();

//  ESP32Processor* ProcessFactory(const char* name,const char* message_format,const char* message_transport);
  ESP32Processor* ProcessFactory(JsonObject &process);
  ESP32Connection* ConnectionFactory(JsonObject &connection);

  void SetupProcessors(JsonObject&);
  void SetupConnections(JsonObject&);

  void ConfigureSDCard();


  // Action functions
  void SetTime();
  void CheckFTP();
  void LogData();
  void Restart();

  // Tracker state update
  void Update();
  void PrintError(int e, String err_string);

  // Data processing
  void IMUAcquisition();
  void Acquisition();
  void UpdateDataMessage();
  // void SensorFusion();

  // Data Indicator
  void Indicator(int nflash, int led_on_ms, int led_off_ms,int led_end_pause);


  /**
  * Variables to go in here
  */
  enum class State{ Initialize, Acquisition, Transfer};

  /**
  *
  */
  // std::vector<VMessage* message> messages;


  std::vector< ESP32Processor* > fProcessors;
  std::vector< ESP32Connection* > fConnections;

  // Set up a Geolocation variable to track your current Location
  Geolocation currentLocation;

  int fHours;
  int fMinutes;
  int fSeconds;
  int fMilliseconds;

  int fState;
  bool fStateUpdated;
  bool fAccelStateUpdated;
  bool fGyroStateUpdated;
  bool fMagStateUpdated;
  bool fGPSStateUpdated;

  float ax,ay,az,gx,gy,gz,mx,my,mz,temperature,lat,lon;

  // For the Sensor Fusion
  // SF filter;
  // ESP32SensorFusion fSensorFusion;

  float pitch, roll, yaw;
  float deltat;

  bool accel_data_available ;
  bool gyro_data_available ;
  bool mag_data_available ;
  bool gps_data_available ;
  bool temperature_data_available;

  long fMessageCount;
  long fStartTime;
  long fCurrentTime;
  long fLastTime;

  ESP32Configuration *fESP32Config;
  State ESP32_State;


  bool fDataTransferEnable;

  bool fSilentRunning;

  int fLEDState;

  long fRescueCount;

  float fDataRate;

  String fOutputMessage;


  float fMeasurementInterval;



};

#endif /* ESP32Configuration_hpp */
