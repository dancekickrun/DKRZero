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

#include "ESP32TrackerManager.h"

ESP32TrackerManager::ESP32TrackerManager() : ESP32_State(State::Initialize)
{

  pinMode(0, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  // Aux led
  pinMode(13, OUTPUT);

  /**
  * Defaults go here
  */
  fSilentRunning = false;
  fStateUpdated = false;
  fMessageCount = 0 ;
  // fSensorFusionEnable = false;
  fMeasurementInterval = -1;

  AccelSetup=false;
  GyroSetup=false;
  MagSetup=false;


  // For button 0 to enter another mode (i.e. listening for configuration updates)
  // const int BUTTON_PIN = 0;
  // pinMode(BUTTON_PIN, INPUT_PULLUP);

  fLEDState = LOW;

  // Serial communications
  Serial.begin(115200);


  // Read the configuration file and initialize the processors vector

  ax=0;ay=0;az=0;gx=0;gy=0;gz=0;mx=0;my=0;mz=0;

};

// void ESP32TrackerManager::CheckFTP(){
//
//   // fESP32Config->GetFTPServer()->handleFTP(); //make sure in loop you call handleFTP()!!
// }


void ESP32TrackerManager::SetTime(){

  long day = 86400000; // 86400000 milliseconds in a day
  long hour = 3600000; // 3600000 milliseconds in an hour
  long minute = 60000; // 60000 milliseconds in a minute
  long second =  1000; // 1000 milliseconds in a second
  long millisecond =  1; // 1000 milliseconds in a second

  if(ESP32_State == State::Initialize){
    fStartTime = millis();
    fHours = (fCurrentTime % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
    fMinutes = ((fCurrentTime % day) % hour) / minute ;         //and so on...
    fSeconds = (((fCurrentTime % day) % hour) % minute) / second;
    fMilliseconds = ((((fCurrentTime % day) % hour) % minute) % second) / millisecond;
  } else {
    fCurrentTime = millis();
    // int days = timeNow / day ;                                //number of days
    // fHours = (fCurrentTime % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
    // fMinutes = ((fCurrentTime % day) % hour) / minute ;         //and so on...
    // fSeconds = (((fCurrentTime % day) % hour) % minute) / second;
    // fMilliseconds = ((((fCurrentTime % day) % hour) % minute) % second) / millisecond;
  }


}

/*
*
* [ESP32TrackerManager::ConfigureSDCard ]
* The SD card needs to be mounted in order to read the configuration file config.txt
*
*/
void ESP32TrackerManager::ConfigureSDCard()
{
  // Mount SD Card
  if(!SD.begin(33, SPI, 200*10000000, "/sd")) PrintError(Error::SDCardMountFail, String("Card Mount Failed."));
  return;
  Serial.println("INFO: SD Card Mounted Successfully ");

  // Get the card type
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) PrintError(Error::NoSDCard, String("No SD Card found -> Please insert a SD card with an initialisation file."));

    Serial.print("INFO: SD Card Type: ");
    if(cardType == CARD_MMC){
      Serial.println("MMC");
      // fInfoFile.print("MMC");
    } else if(cardType == CARD_SD){
      Serial.println("SDSC");
      // fInfoFile.print("SDSC");
    } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
      // fInfoFile.print("SDHC");
    } else {
      Serial.println("UNKNOWN");
      // fInfoFile.print("UNKNOWN");
    }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("INFO: SD Card Size: %lluMB\n", cardSize);

};


/**
 * [ESP32TrackerManager::Configure]
 * This is the entry point for starting up the unit
 */
void ESP32TrackerManager::Configure(){

  // Flash the lights before start-up
  digitalWrite(5, 1);
  delay(3000);
  digitalWrite(5, 0);
  digitalWrite(13, 1);
  delay(3000);
  digitalWrite(13, 0);

  Serial.println("INFO: Configuring SD Card");
  // Open the SD Card
  ConfigureSDCard();


  /**
  * This reads and parses  the config.txt file
  */
  // Open file for reading
  const char *filename = "/config.txt";

  // Load the JSON file
  File file = SD.open(filename);

  // Check if the file hasn't been found here
  if(!file) PrintError(Error::ConfigOpenFail, String("Failed to open configuration file."));

  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t bufferSize = JSON_OBJECT_SIZE(28) + 780;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  // Parse the root object
  JsonObject& root = jsonBuffer.parseObject(file);

  // Throw error if fail
  if (!root.success()) PrintError(Error::ConfigReadFail, String("Failed to read configuration file."));

  // Comms with the GPS unit
  // fESP32Config->GetGPSSerial()->begin(9600);

  // - Setup using the configuration class
  // fESP32Config->Configure(root);

  // Get the desired data rate (if -1 then free running)
  if(root.containsKey("desired_data_rate")){
    fDesiredDataRate = (float) root["desired_data_rate"];
  } else {
    fDesiredDataRate = -1;
  }
  // Need to check if this json field exists first?
  Serial.print("INFO: Desired Sample Rate is "); Serial.println(fDesiredDataRate);

  // Set up the sensors
  SetupSensors(root);
  // Set up the connections
  SetupConnections(root);
  // Now get the data processors
  SetupProcessors(root);

  // ESP32 should be configured by now
  fMeasurementInterval = 1000.0/fDesiredDataRate;// Rate in s-1
  Serial.print("INFO: Measurement Interval (ms) "); Serial.println(fMeasurementInterval);

  // Initialize the time
  SetTime();
  fLastTime=fCurrentTime;

  // ... and the IMU unit
  StartIMU();
  delay(100);

  // Change the state of the unit to acquire data
  ESP32_State = State::Acquisition;

}

void ESP32TrackerManager::PrintError(int e, String err_string)
{

  while(1){
    Serial.println(String("ERROR: ") + err_string);
    Indicator(e+2,100,100,1000);
  }

}




void ESP32TrackerManager::SetupSensors(JsonObject &root)
{
  Serial.println("");
  Serial.println("===============================");
  Serial.println("INFO: Setting up sensors");


  // Look for the sensors block
  JsonArray& json_sensors = root["sensors"];
  for (auto& sensor : json_sensors)
  {
    fSensors.push_back(SensorFactory::ConstructSensor(sensor));

    // Print the type
    Serial.print("INFO: Found sensor type "); Serial.println((int) (fSensors.back())->GetType());

  }

  Serial.print("INFO: Found "); Serial.print((int) fSensors.size()); Serial.println(" sensors");
  Serial.println("===============================");
  Serial.println("");

  // SetupGyroscope(root);
  // SetupAccelerometer(root);
  // SetupMagnetometer(root);
  // SetupGPS(root);
  // SetupTemperature(root);
  // SetupMessenger(root);

}


void ESP32TrackerManager::SetupConnections(JsonObject &root)
{
  Serial.println("===============================");
  Serial.println("INFO: Setting up connections");

  // Loop over all elements that are within the processors
  JsonArray& json_connections = root["connections"];
  for (auto& connection : json_connections) fConnections.push_back(ConnectionFactory::ConstructConnection(connection));

  Serial.println("===============================");
  Serial.println("");
}


void ESP32TrackerManager::SetupProcessors(JsonObject &root)
{
  Serial.println("");
  Serial.println("===============================");
  Serial.println("INFO: Setting up processors");

  // Loop over all elements that are within the processors
  JsonArray& json_processors = root["processors"];
  for (auto& process : json_processors) fProcessors.push_back(ProcessFactory::ConstructProcess(process));

  Serial.println("===============================");
  Serial.println("");

}








// ESP32Connection* ESP32TrackerManager::ConnectionFactory(JsonObject &connection)
// {
//
//   // Get the connection type from the config txt
//   const char* type = connection["type"];
//
//   // connections to check against
//   const char* wifi_connection_name = "wifi";
//   const char* ble_connection_name = "ble";
//   const char* sdcard_connection_name = "sdcard";
//
//   // For wifi connections
//   if (strcmp(type, wifi_connection_name) == 0){
//
//     // Find the password and ssid
//     const char* ssid =  connection["ssid"];
//     const char* password =  connection["password"];
//     int attempts = connection["attempts"];
//
//     // Check if the user would like a ftp server
//     bool ftp_enable = (bool) connection["ftp_enable"];
//     //
//     if(ftp_enable){
//       return (new ESP32FTPConnection(ssid,password,attempts));
//     } else {
//       // return a wifi connection
//       return (new ESP32WifiConnection(ssid,password,attempts));
//     }
//
//
//   }
//   else if (strcmp(type, ble_connection_name) == 0)
//   {
//     Serial.println("INFO: Setting up BLE connection");
//
//     // const char* device_uuid = connection["characteristic_uuid"];
//     // const char* service_uuid = connection["service_uuid"];
//     // const char* mode = connection["mode"];
//
//     // return a ble connection
//     return new ESP32BLEConnection();
//
//   }
//   else if(strcmp(type, sdcard_connection_name) == 0)
//   {
//
//     const char* stub =  connection["stub_filename"];
//     int cont_acq_char =  (int) connection["continuous_acquisition"];
//     bool cont_acq =  (bool) cont_acq_char;
//
//     Serial.println("INFO: Setting up SD card connection");
//
//     Serial.print("INFO: Using stub filename ");
//     Serial.println(stub);
//
//     Serial.print("INFO: Using continous bool flag ");
//     Serial.println(cont_acq);
//
//     return new ESP32SDCardConnection(stub, cont_acq);
//
//     // // Check if empty
//     // if((stub==NULL)&&(cont_acq_char==NULL)){
//     //   Serial.println("INFO: Setting up SD card connection");
//     //   return new ESP32SDCardConnection();
//     // }
//     //
//     // if((stub!=NULL)&&(cont_acq_char==NULL))
//     // {
//     //   Serial.println("INFO: Setting up SD card connection");
//     //   Serial.print("INFO: Using stub filename ");
//     //   Serial.println(stub);
//     //   return new ESP32SDCardConnection(stub);
//     // }
//     //
//     // if((stub==NULL)&&(cont_acq_char!=NULL))
//     // {
//     //   Serial.println("INFO: Setting up SD card connection");
//     //   Serial.print("INFO: Using continous bool flag ");
//     //   Serial.println(cont_acq);
//     //   return new ESP32SDCardConnection(cont_acq);
//     // }
//     //
//     // if((stub!=NULL)&&(cont_acq_char!=NULL))
//     // {
//     //   Serial.println("INFO: Setting up SD card connection");
//     //   Serial.print("INFO: Using stub filename ");
//     //   Serial.println(stub);
//     //   Serial.print("INFO: Using continous bool flag ");
//     //   Serial.println(cont_acq);
//     //   return new ESP32SDCardConnection(stub, cont_acq);
//     // }
//
//   }
//
// }



// For those processors that don't require a parameter
// ESP32Processor* ESP32TrackerManager::ProcessFactory(JsonObject &process)
// {
//
//   const char* name = process["name"];
//   const char* message_format = process["message_format"];
//   const char* message_transport = process["message_transport"];
//
//   // Constant char strings for the processors available
//   const char* cadence_process_name = "cadence";
//   const char* sf_process_name = "sensorfusion";
//   const char* geo_process_name = "geolocation";
//   const char* raw_process_name = "raw";
//   const char* impulse_process_name = "impulse";
//   const char* activity_process_name = "activity";
//
//   // For characterization
//   const char* sample_freq_process_name = "sample_frequency";
//   const char* sample_stats_process_name = "sample_statistics";
//
//   Serial.print("INFO: Setting up processor ");
//   Serial.println(name);
//
//   // Return the Class of the processor if requested in the JSON
//   if (strcmp(name, cadence_process_name) == 0) return new ESP32Cadence(message_format,message_transport);
//
//   if (strcmp(name, activity_process_name) == 0){
//
//     float low_threshold = 0.1;
//     float mid_threshold = 0.5;
//     float hi_threshold = 1.0;
//
//     const char* buf_low_thresh = 0;
//     buf_low_thresh = process["low_threshold"];
//     if(buf_low_thresh[0] != '\0')  low_threshold = String(buf_low_thresh).toFloat();
//
//     const char* buf_mid_thresh = 0;
//     buf_mid_thresh = process["mid_threshold"];
//     if(buf_mid_thresh[0] != '\0')  mid_threshold = String(buf_mid_thresh).toFloat();
//
//
//     const char* buf_hi_thresh = 0;
//     buf_hi_thresh = process["high_threshold"];
//     if(buf_hi_thresh[0] != '\0')  hi_threshold = String(buf_hi_thresh).toFloat();
//
//     return new ESP32Activity(message_format,message_transport, low_threshold, mid_threshold, high_threshold);
//   }
//
//   if (strcmp(name, sample_freq_process_name) == 0){
//     int npoints = atoi(process["num_points"]);// Need to check if this exists
//     return (new ESP32SampleFrequency(message_format,message_transport,npoints));
//   }
//
//   if (strcmp(name, sample_stats_process_name) == 0){
//     int npoints = atoi(process["num_points"]);// Need to check if this exists
//     std::vector<bool> onoff(9,true);
//     onoff[0] = (bool) process["ax_enable"];onoff[1] = (bool) process["ay_enable"];onoff[2] = (bool) process["az_enable"];
//     onoff[3] = (bool) process["gx_enable"];onoff[4] = (bool) process["gy_enable"];onoff[5] = (bool) process["gz_enable"];
//     onoff[6] = (bool) process["mx_enable"];onoff[7] = (bool) process["my_enable"];onoff[8] = (bool) process["mz_enable"];
//     return (new ESP32SampleStatistics(message_format,message_transport,npoints,onoff));
//   }
//
//
//   if (strcmp(name, sf_process_name) == 0) return new ESP32SensorFusion(true,false,message_format,message_transport);
//   // if (strcmp(name, geo_process_name) == 0) return new ESP32Geolocation(message_format,message_transport);
//   if (strcmp(name, raw_process_name) == 0) return new ESP32RawData(message_format,message_transport);
//   if (strcmp(name, impulse_process_name) == 0)
//   {
//
//     int buf_size = atoi(process["buffer_size"]);
//     bool accel_enable = (bool) process["accel_enable"];
//     bool gyro_enable = (bool) process["gyro_enable"];
//     bool mag_enable = (bool) process["mag_enable"];
//
//     int buf_delay = atoi(process["trigger_delay"]);
//     int coord = atoi(process["trigger_coord"]);
//     const char* buf_thresh = process["trigger_threshold"];
//     float threshold = String(buf_thresh).toFloat();
//     std::vector<bool> enables = {accel_enable,gyro_enable,mag_enable};
//
//     return new ESP32Impulse(message_format, message_transport, buf_size, threshold,  buf_delay, enables, coord);
//   }
//
//   // Need to check that something has been selected here
//   Serial.println("INFO: Unknown Processor");
//
// }





// void ESP32TrackerManager::SetupConnections(JsonObject &root)
// {
//   Serial.println("INFO: Setting up connections");
//   // Loop over all elements that are within the processors
//   JsonArray& json_connections = root["connections"];
//
//   fDataTransferEnable = false;
//
//   for (auto& connection : json_connections){
//
//     fConnections.push_back(ConnectionFactory(connection));
//     Serial.println("INFO: Initializing connections");
//     (fConnections.back())->Initialize();
//     Serial.println("INFO: Starting connections");
//     (fConnections.back())->Start();
//
//     // Check if we're able to transfer data using the Web or Wifi
//     fDataTransferEnable = fDataTransferEnable || (fConnections.back())->IsType(ESP32Connection::ConnectionType::Wifi);
//     fDataTransferEnable = fDataTransferEnable || (fConnections.back())->IsType(ESP32Connection::ConnectionType::FTP);
//     fDataTransferEnable = fDataTransferEnable || (fConnections.back())->IsType(ESP32Connection::ConnectionType::BLE);
//
//   }
//
//   if(fDataTransferEnable){
//     Serial.println("INFO: Data transfer is enabled");
//   } else {
//     Serial.println("INFO: Data transfer is not enabled");
//   }
//
// }


// void ESP32TrackerManager::SetupProcessors(JsonObject &root)
// {
//   // Loop over all elements that are within the processors
//   JsonArray& json_processors = root["processors"];
//
//   for (auto& process : json_processors){
//
//     // Check whether it is a process that accepts arguments
//     // fProcessors.push_back(ProcessFactory(process["name"], process["message_format"], process["message_transport"]));
//     // (fProcessors.back())->Setup();
//     fProcessors.push_back(ProcessFactory(process));
//
//     // Setup the processor
//     (fProcessors.back())->Setup();
//
//   }
// };




void ESP32TrackerManager::StartIMU()
{


  // Loop over the sensors
  for(auto sensor: fSensors)
  {
    // Print the type
    Serial.print("INFO: Starting IMU sensor "); Serial.println((int) sensor->GetType());

    // Get the sensor type
    if(sensor->IsType(ESP32Sensor::SensorType::Accelerometer))
    {
      sensor->ConfigureIMU(&imu);
      AccelSetup=true;
    }

    if(sensor->IsType(ESP32Sensor::SensorType::Gyroscope))
    {
      sensor->ConfigureIMU(&imu);
      GyroSetup=true;
    }

    if(sensor->IsType(ESP32Sensor::SensorType::Magnetometer))
    {
      sensor->ConfigureIMU(&imu);
      MagSetup=true;
    }


  }

  bool all_sensors_setup  = AccelSetup && MagSetup && GyroSetup;
  if(AccelSetup){ Serial.println("INFO: Accelerometer definition ok "); } else { Serial.println("INFO: Accelerometer definition not ok "); }
  if(MagSetup){ Serial.println("INFO: Magnetometer definition ok "); } else { Serial.println("INFO: Magnetometer definition not ok "); }
  if(GyroSetup){ Serial.println("INFO: Gyroscope definition ok "); } else { Serial.println("INFO: Gyroscope definition not ok "); }

  if(!all_sensors_setup) PrintError(Error::NotAllSensorsDefined, String("Sensor block definitions incomplete"));

  if(!imu.begin()) PrintError(Error::IMUCommsFail, String("Failed to communicate with LSM9DS1."));

  Serial.println("INFO: IMU Started");

}

//
// "processors": [ "sensor_fusion", "cadence" ]
//
// void ESP32TrackerManager::Process(){
//
//   // Loop over all processes
//
// }


void ESP32TrackerManager::Acquisition()
{

  // Get the raw data
  SetTime();
  IMUAcquisition();

  // Serial.print("State Updated "); Serial.println(fStateUpdated);
  if(!fStateUpdated) return;

  // Loop over the processors
  unsigned long startMillis = millis();
  unsigned long cur = millis();
  for(auto process: fProcessors){

    // startMillis = millis();
    process->ProcessData(fCurrentTime,lat,lon,ax,ay,az,gx,gy,gz,mx,my,mz);
    // cur = millis();
    // Serial.print("ProcessData ");
    // Serial.println(cur-startMillis);

    // fOutputMessage+=process->Message();
    // fOutputMessage+="\n";
    // fMessageCount++;
    // if((fMessageCount%20)==0)
    // {
    //   Serial.println(fOutputMessage);
    //   fOutputMessage="";
    // }

    // startMillis = millis();
    if(process->GetDataReady())
    {
      // Serial.println(process->Message());
      for(auto connection: fConnections)
      {
        connection->Communicate(process->Message());
      }
    }
    // cur = millis();
    // Serial.print("Communicate ");
    // Serial.println(cur-startMillis);



  }

  // Get the message
  // if(fStateUpdated) for(auto process: fProcessors) process->Communicate(fESP32Config);

  // Serial.println("");


}

void ESP32TrackerManager::IMUAcquisition()
{
  float tmpx,tmpy,tmpz;

  // Read the sensors
  if(imu.tempAvailable())
  {
    imu.readTemp();
    temperature = imu.temperature;
  }

  //   accel_data_available = fESP32Config->GetAccelerometerEnable(0) && fESP32Config->GetIMU()->accelAvailable();
  //   gyro_data_available = fESP32Config->GetGyroscopeEnable() && fESP32Config->GetIMU()->gyroAvailable();
  //   mag_data_available = fESP32Config->GetMagnetometerEnable()  && fESP32Config->GetIMU()->magAvailable();
  //   gps_data_available = fESP32Config->GetGPSEnable()  && fESP32Config->GetGPS()->read();
  //   temperature_data_available = fESP32Config->GetTemperatureEnable() && fESP32Config->GetIMU()->tempAvailable();

  // Read Accelerometer
  if(imu.accelAvailable())
  {
    imu.readAccel();
    tmpx = ax;
    tmpy = ay;
    tmpz = az;
    ax = imu.calcAccel(imu.ax);
    ay = imu.calcAccel(imu.ay);
    az = imu.calcAccel(imu.az);
    fAccelStateUpdated = !( (ax==tmpx) && (ay==tmpy) && (az==tmpz) );
  }

  // Read the gyro
  if(imu.gyroAvailable())
  {
    imu.readGyro();
    tmpx = gx;
    tmpy = gy;
    tmpz = gz;
    gx = imu.calcGyro(imu.gx);
    gy = imu.calcGyro(imu.gy);
    gz = imu.calcGyro(imu.gz);
    fGyroStateUpdated = !( (gx==tmpx) && (gy==tmpy) && (gz==tmpz) );
  }

  // Read the magnetometer
  if(imu.magAvailable())
  {
    tmpx = mx;
    tmpy = my;
    tmpz = mz;
    imu.readMag();
    mx = imu.calcMag(imu.mx);
    my = imu.calcMag(imu.my);
    mz = imu.calcMag(imu.mz);
    fMagStateUpdated = !( (mx==tmpx) && (my==tmpy) && (mz==tmpz) );
  }

  // TODO Insert code to read GPS
  fState = fAccelStateUpdated
  + 2*fGyroStateUpdated
  + 4*fMagStateUpdated
  + 8*fGPSStateUpdated;

  fStateUpdated = fAccelStateUpdated || fGyroStateUpdated || fMagStateUpdated || fGPSStateUpdated;


}



// void ESP32TrackerManager::IMUAcquisition(){
//   // if(!fSilentRunning) Serial.println("IMUAcquisition()");
//   fAccelStateUpdated = false;
//   fGyroStateUpdated = false;
//   fMagStateUpdated = false;
//   fGPSStateUpdated = false;
//
//   accel_data_available = fESP32Config->GetAccelerometerEnable(0) && fESP32Config->GetIMU()->accelAvailable();
//   gyro_data_available = fESP32Config->GetGyroscopeEnable() && fESP32Config->GetIMU()->gyroAvailable();
//   mag_data_available = fESP32Config->GetMagnetometerEnable()  && fESP32Config->GetIMU()->magAvailable();
//   gps_data_available = fESP32Config->GetGPSEnable()  && fESP32Config->GetGPS()->read();
//   temperature_data_available = fESP32Config->GetTemperatureEnable() && fESP32Config->GetIMU()->tempAvailable();
//
//
//
//   if(temperature_data_available){
//     fESP32Config->GetIMU()->readTemp();
//     temperature = fESP32Config->GetIMU()->temperature;
//   } else {
//     temperature = -1;
//   }
//
//   if(accel_data_available){
//     fESP32Config->GetIMU()->readAccel();
//     float tmpx = ax;
//     float tmpy = ay;
//     float tmpz = az;
//     ax = fESP32Config->GetIMU()->calcAccel(fESP32Config->GetIMU()->ax);
//     ay = fESP32Config->GetIMU()->calcAccel(fESP32Config->GetIMU()->ay);
//     az = fESP32Config->GetIMU()->calcAccel(fESP32Config->GetIMU()->az);
//     fAccelStateUpdated = !( (ax==tmpx) && (ay==tmpy) && (az==tmpz) );
//   }
//   if(gyro_data_available){
//     fESP32Config->GetIMU()->readGyro();
//     float tmpx = gx;
//     float tmpy = gy;
//     float tmpz = gz;
//     gx = fESP32Config->GetIMU()->calcGyro(fESP32Config->GetIMU()->gx);
//     gy = fESP32Config->GetIMU()->calcGyro(fESP32Config->GetIMU()->gy);
//     gz = fESP32Config->GetIMU()->calcGyro(fESP32Config->GetIMU()->gz);
//     fGyroStateUpdated = !( (gx==tmpx) && (gy==tmpy) && (gz==tmpz) );
//   }
//   if (mag_data_available){
//     float tmpx = mx;
//     float tmpy = my;
//     float tmpz = mz;
//     fESP32Config->GetIMU()->readMag();
//     mx = fESP32Config->GetIMU()->calcMag(fESP32Config->GetIMU()->mx);
//     my = fESP32Config->GetIMU()->calcMag(fESP32Config->GetIMU()->my);
//     mz = fESP32Config->GetIMU()->calcMag(fESP32Config->GetIMU()->mz);
//     fMagStateUpdated = !( (mx==tmpx) && (my==tmpy) && (mz==tmpz) );
//   }
//   if(gps_data_available){
//     digitalWrite(13, 1);
//     currentLocation = fESP32Config->GetGPS()->getGeolocation();
//     float tmplat = lat;
//     float tmplon = lon;
//     lat = (float) currentLocation.latitude;
//     lon = (float) currentLocation.longitude;
//     fGPSStateUpdated = !( (lon==tmplon) && (lat==tmplat) );
//   } else {
//     digitalWrite(13, 0);
//   }
//
//   fState = fAccelStateUpdated
//   + 2*fGyroStateUpdated
//   + 4*fMagStateUpdated
//   + 8*fGPSStateUpdated;
//
//   fStateUpdated = fAccelStateUpdated || fGyroStateUpdated || fMagStateUpdated || fGPSStateUpdated;
//
//   // if(fStateUpdated && fSensorFusionEnable) SensorFusion();
//
//   if(!fStateUpdated){
//     // Check whether we've lost touch with the data and reset if we have
//     fRescueCount+=1;
//     if(fRescueCount>100) Restart();
//   } else {
//     fRescueCount = 0;
//   }
//
// }

void ESP32TrackerManager::Update(){

  SetTime();

  switch (ESP32_State)
  {

    case State::Initialize:
    Configure();
    break;


    case State::Acquisition:

    // Check whether free-running acquisition or not
    if(fMeasurementInterval>0){

      // Check if the chosen time has elapsed for the data rate
      if ((fCurrentTime - fLastTime) >= (long) fMeasurementInterval)
      {
        // Save the last time you blinked the LED
        fLastTime = fCurrentTime;
        // Acquire the data
        Acquisition();
      }

    }else{

      // Acquire the data in a free-running mode
      Acquisition();

    }


    // To transfer into FTP mode
    // Check if button 0 has been pressed
    if( (digitalRead(0) == LOW) )
    { // Check if button has been pressed
      while (digitalRead(0) == LOW); // Wait for button to be released
      Serial.println("INFO: Pausing data acquisition");
      ESP32_State = State::Paused;
    }

    break;


    case State::Paused:

    // Every second flash the LED
    if ((fCurrentTime - fLastTime) > 1000)
    {
      fLastTime = fCurrentTime;
      if (fLEDState == LOW)
      {
        fLEDState = HIGH;
      } else {
        fLEDState = LOW;
      }
      digitalWrite(5, fLEDState);
    }

    // Check if button has been pressed
    if( (digitalRead(0) == LOW) )
    {
      while (digitalRead(0) == LOW); // Wait for button to be released
      Serial.println("INFO: Changing to data acquisition mode");

      // Turn off the LED
      if (fLEDState == HIGH) fLEDState = LOW;
      digitalWrite(5, fLEDState);
      ESP32_State = State::Acquisition;
    }

    // case State::Transfer:
    //
    // // Every second flash the LED
    // if ((fCurrentTime - fLastTime) > 1000)
    // {
    //   fLastTime = fCurrentTime;
    //   if (fLEDState == LOW)
    //   {
    //     fLEDState = HIGH;
    //   } else {
    //     fLEDState = LOW;
    //   }
    //   digitalWrite(5, fLEDState);
    // }
    //
    // // Update the FTP connection
    // // CheckFTP();
    // // delay(50);
    //
    // // Check if button has been pressed
    // if( (digitalRead(0) == LOW) )
    // {
    //   while (digitalRead(0) == LOW); // Wait for button to be released
    //   Serial.println("INFO: Changing to data acquisition mode");
    //
    //   // Turn off the LED
    //   if (fLEDState == HIGH) fLEDState = LOW;
    //   digitalWrite(5, fLEDState);
    //
    //   // Disconnect from wifi if MQTT is false and WIFI is true
    //   // if((!fESP32Config->GetMQTTEnable()) && (fESP32Config->GetWifiEnable()))
    //   //     fESP32Config->DisconnectWifi();
    //
    //   ESP32_State = State::Acquisition;
    //
    // }

    break;

  }


  fStateUpdated = false;

  // if(fCurrentTime>10000) Reset();

}


void ESP32TrackerManager::Restart(){ ESP.restart();}



void ESP32TrackerManager::Indicator(int nflash, int led_on_ms=250, int led_off_ms=250,int led_end_pause=1000){

  const int LED_PIN = 5;

  // Put status to flashing code here:
  for(int i=0;i<nflash;i++){
    digitalWrite(LED_PIN, HIGH); // Turn on LED
    delay(led_on_ms);
    digitalWrite(LED_PIN, LOW); // Turn on LED
    delay(led_off_ms);
  }
  delay(led_end_pause);

}
