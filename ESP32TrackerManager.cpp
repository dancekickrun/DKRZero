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

  // For button 0 to enter another mode (i.e. listening for configuration updates)
  // const int BUTTON_PIN = 0;
  // pinMode(BUTTON_PIN, INPUT_PULLUP);

  fLEDState = LOW;

  // Serial communications
  Serial.begin(115200);


  // Read the configuration file and initialize the processors vector

  ax=0;ay=0;az=0;gx=0;gy=0;gz=0;mx=0;my=0;mz=0;

};

void ESP32TrackerManager::CheckFTP(){

  // fESP32Config->GetFTPServer()->handleFTP(); //make sure in loop you call handleFTP()!!
}


void ESP32TrackerManager::SetTime(){

  long day = 86400000; // 86400000 milliseconds in a day
  long hour = 3600000; // 3600000 milliseconds in an hour
  long minute = 60000; // 60000 milliseconds in a minute
  long second =  1000; // 1000 milliseconds in a second
  long millisecond =  1; // 1000 milliseconds in a second

  if(ESP32_State == State::Initialize){
    fStartTime = millis();
  } else {
    fCurrentTime = millis();
    // int days = timeNow / day ;                                //number of days
    fHours = (fCurrentTime % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
    fMinutes = ((fCurrentTime % day) % hour) / minute ;         //and so on...
    fSeconds = (((fCurrentTime % day) % hour) % minute) / second;
    fMilliseconds = ((((fCurrentTime % day) % hour) % minute) % second) / millisecond;
  }


}

void ESP32TrackerManager::ConfigureSDCard()
{

  //Call begin with (cs pin, SPI, rate (up to 10MHz), "/sd")
  // Mount SD Card
  if(!SD.begin(33, SPI, 200*10000000, "/sd")){
    PrintError(Error::ConfigReadFail, String("Card Mount Failed."));
    // Setup error codes:
    //
    // # Fast Flashes
    //    3                  Cannot start IMU
    //    4                  Cannot mount SD
    // while (1){Indicator(4,100,100,1000);};
    // digitalWrite(ESP_LED_PIN, 0);

    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD Card found -> Please insert a SD card with an initialisation file");
    while(1);
    return;
  }

  if(!fSilentRunning) {
    Serial.print("INFO: SD Card Type: ");
    // fInfoFile.print("INFO: SD Card Type: ");
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
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  if(!fSilentRunning) Serial.printf("SD Card Size: %lluMB\n", cardSize);

  //call begin with device, CS pin, port, frequency, and mount point.
  // if(SDSerialFile.begin(&SD, 33, SPI, 10000000, "/sd") == 0)
  // {
  //   Serial.println("SD begin did not succeed, halting.");
  //   digitalWrite(ESP_LED_PIN, 0);
  //   while(1);
  // }

};

void ESP32TrackerManager::Configure(){

  digitalWrite(5, 1);
  delay(3000);
  digitalWrite(5, 0);
  digitalWrite(13, 1);
  delay(3000);
  digitalWrite(13, 0);

  // Open the SD Card
  ConfigureSDCard();

  /**
  * This reads and parses  the config.txt file
  */

  // Open file for reading
  const char *filename = "/config.txt";

  // Load the JSON file
  File file = SD.open(filename);
  if(!file) PrintError(Error::ConfigReadFail, String("Failed to open configuration file."));
  // Check if the file hasn't been found here
  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t bufferSize = JSON_OBJECT_SIZE(28) + 780;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  // Parse the root object
  JsonObject& root = jsonBuffer.parseObject(file);

  // Throw error if fail
  if (!root.success()) PrintError(Error::ConfigReadFail, String("Failed to read configuration file."));

  // Initialize the ESP32 board
  // - This also sets up
  // - - FTP
  // - - Data log files
  fESP32Config = new ESP32Configuration();

  // Comms with the GPS unit
  fESP32Config->GetGPSSerial()->begin(9600);

  // - Setup from file
  // - - Sensors
  // - - Wifi
  fESP32Config->Configure(root);

  SetupConnections(root);
  // Now get the data processors
  SetupProcessors(root);

  // ESP32 should be configured by now
  fMeasurementInterval = 1000.0/fESP32Config->GetSampleRate();// Rate in s-1
  Serial.print("INFO: Measurement Interval (ms) "); Serial.println(fMeasurementInterval);

  SetTime();

  StartIMU();
  delay(100);

  ESP32_State = State::Acquisition;

}

void ESP32TrackerManager::PrintError(int e, String err_string)
{

  while(1){
    Serial.println(String("ERR: ") + err_string);
    Indicator(e+2,100,100,1000);
  }

}


ESP32Connection* ESP32TrackerManager::ConnectionFactory(JsonObject &connection)
{

  // Get the connection type from the config txt
  const char* type = connection["type"];

  // connections to check against
  const char* wifi_connection_name = "wifi";
  const char* ble_connection_name = "ble";
  const char* sdcard_connection_name = "sdcard";

  // For wifi connections
  if (strcmp(type, wifi_connection_name) == 0){

    // Find the password and ssid
    const char* ssid =  connection["ssid"];
    const char* password =  connection["password"];
    int attempts = connection["attempts"];

    // Check if the user would like a ftp server
    bool ftp_enable = (bool) connection["ftp_enable"];
    //
    if(ftp_enable){
      return (new ESP32FTPConnection(ssid,password,attempts));
    } else {
      // return a wifi connection
      return (new ESP32WifiConnection(ssid,password,attempts));
    }


  }
  else if (strcmp(type, ble_connection_name) == 0)
  {
    Serial.println("INFO: Setting up BLE connection");

    // const char* device_uuid = connection["characteristic_uuid"];
    // const char* service_uuid = connection["service_uuid"];
    // const char* mode = connection["mode"];

    // return a ble connection
    return new ESP32BLEConnection();

  }
  else if(strcmp(type, sdcard_connection_name) == 0)
  {

    const char* stub =  connection["stub_filename"];
    int cont_acq_char =  (int) connection["continuous_acquisition"];
    bool cont_acq =  (bool) cont_acq_char;

    Serial.println("INFO: Setting up SD card connection");

    Serial.print("INFO: Using stub filename ");
    Serial.println(stub);

    Serial.print("INFO: Using continous bool flag ");
    Serial.println(cont_acq);

    return new ESP32SDCardConnection(stub, cont_acq);

    // // Check if empty
    // if((stub==NULL)&&(cont_acq_char==NULL)){
    //   Serial.println("INFO: Setting up SD card connection");
    //   return new ESP32SDCardConnection();
    // }
    //
    // if((stub!=NULL)&&(cont_acq_char==NULL))
    // {
    //   Serial.println("INFO: Setting up SD card connection");
    //   Serial.print("INFO: Using stub filename ");
    //   Serial.println(stub);
    //   return new ESP32SDCardConnection(stub);
    // }
    //
    // if((stub==NULL)&&(cont_acq_char!=NULL))
    // {
    //   Serial.println("INFO: Setting up SD card connection");
    //   Serial.print("INFO: Using continous bool flag ");
    //   Serial.println(cont_acq);
    //   return new ESP32SDCardConnection(cont_acq);
    // }
    //
    // if((stub!=NULL)&&(cont_acq_char!=NULL))
    // {
    //   Serial.println("INFO: Setting up SD card connection");
    //   Serial.print("INFO: Using stub filename ");
    //   Serial.println(stub);
    //   Serial.print("INFO: Using continous bool flag ");
    //   Serial.println(cont_acq);
    //   return new ESP32SDCardConnection(stub, cont_acq);
    // }

  }

}



// For those processors that don't require a parameter
ESP32Processor* ESP32TrackerManager::ProcessFactory(JsonObject &process)
{

  const char* name = process["name"];
  const char* message_format = process["message_format"];
  const char* message_transport = process["message_transport"];

  // Constant char strings for the processors available
  const char* cadence_process_name = "cadence";
  const char* sf_process_name = "sensorfusion";
  const char* geo_process_name = "geolocation";
  const char* raw_process_name = "raw";
  const char* impulse_process_name = "impulse";

  // For characterization
  const char* sample_freq_process_name = "sample_frequency";
  const char* sample_stats_process_name = "sample_statistics";

  Serial.print("INFO: Setting up processor ");
  Serial.println(name);

  // Return the Class of the processor if requested in the JSON
  if (strcmp(name, cadence_process_name) == 0) return new ESP32Cadence(message_format,message_transport);

  if (strcmp(name, sample_freq_process_name) == 0){
    int npoints = atoi(process["num_points"]);// Need to check if this exists
    return (new ESP32SampleFrequency(message_format,message_transport,npoints));
  }

  if (strcmp(name, sample_stats_process_name) == 0){
    int npoints = atoi(process["num_points"]);// Need to check if this exists
    std::vector<bool> onoff(9,true);
    onoff[0] = (bool) process["ax_enable"];onoff[1] = (bool) process["ay_enable"];onoff[2] = (bool) process["az_enable"];
    onoff[3] = (bool) process["gx_enable"];onoff[4] = (bool) process["gy_enable"];onoff[5] = (bool) process["gz_enable"];
    onoff[6] = (bool) process["mx_enable"];onoff[7] = (bool) process["my_enable"];onoff[8] = (bool) process["mz_enable"];
    return (new ESP32SampleStatistics(message_format,message_transport,npoints,onoff));
  }


  if (strcmp(name, sf_process_name) == 0) return new ESP32SensorFusion(true,false,message_format,message_transport);
  // if (strcmp(name, geo_process_name) == 0) return new ESP32Geolocation(message_format,message_transport);
  if (strcmp(name, raw_process_name) == 0) return new ESP32RawData(message_format,message_transport);
  if (strcmp(name, impulse_process_name) == 0)
  {

    int buf_size = atoi(process["buffer_size"]);
    bool accel_enable = (bool) process["accel_enable"];
    bool gyro_enable = (bool) process["gyro_enable"];
    bool mag_enable = (bool) process["mag_enable"];

    int buf_delay = atoi(process["trigger_delay"]);
    int coord = atoi(process["trigger_coord"]);
    const char* buf_thresh = process["trigger_threshold"];
    float threshold = String(buf_thresh).toFloat();
    std::vector<bool> enables = {accel_enable,gyro_enable,mag_enable};

    return new ESP32Impulse(message_format, message_transport, buf_size, threshold,  buf_delay, enables, coord);
  }

  // Need to check that something has been selected here
  Serial.println("INFO: Unknown Processor");

}

void ESP32TrackerManager::SetupConnections(JsonObject &root)
{
  Serial.println("INFO: Setting up connections");
  // Loop over all elements that are within the processors
  JsonArray& json_connections = root["connections"];

  fDataTransferEnable = false;

  for (auto& connection : json_connections){

    fConnections.push_back(ConnectionFactory(connection));
    Serial.println("INFO: Initializing connections");
    (fConnections.back())->Initialize();
    Serial.println("INFO: Starting connections");
    (fConnections.back())->Start();

    // Check if we're able to transfer data using the Web or Wifi
    fDataTransferEnable = fDataTransferEnable || (fConnections.back())->IsType(ESP32Connection::ConnectionType::Wifi);
    fDataTransferEnable = fDataTransferEnable || (fConnections.back())->IsType(ESP32Connection::ConnectionType::FTP);
    fDataTransferEnable = fDataTransferEnable || (fConnections.back())->IsType(ESP32Connection::ConnectionType::BLE);

  }

  if(fDataTransferEnable){
    Serial.println("INFO: Data transfer is enabled");
  } else {
    Serial.println("INFO: Data transfer is not enabled");
  }

}


void ESP32TrackerManager::SetupProcessors(JsonObject &root)
{
  // Loop over all elements that are within the processors
  JsonArray& json_processors = root["processors"];

  for (auto& process : json_processors){

    // Check whether it is a process that accepts arguments
    // fProcessors.push_back(ProcessFactory(process["name"], process["message_format"], process["message_transport"]));
    // (fProcessors.back())->Setup();

    fProcessors.push_back(ProcessFactory(process));

    // Setup the processor
    (fProcessors.back())->Setup();

  }

  // Check if there is a



};




void ESP32TrackerManager::StartIMU()
{
  if(!fSilentRunning) Serial.println("StartIMU()");

  if (!(fESP32Config->GetIMU())->begin())
  {
    if(!fSilentRunning) PrintError(Error::IMUCommsFail, String("Failed to communicate with LSM9DS1."));
  };

  if(!fSilentRunning) Serial.println("INFO: IMU Started");
  // fInfoFile.println("INFO: IMU Started");
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

  // Run the processors
  // (fProcessors.at(0))->ProcessData(sqrt(ax*ax+ay*ay+az*az));

  if(!fStateUpdated) return;

  // Loop over the processors
  String m;
  for(auto process: fProcessors){
    // For the current processor, do something with the data
    process->ProcessData(fCurrentTime,lat,lon,ax,ay,az,gx,gy,gz,mx,my,mz);
    if(process->GetDataReady()){
      for(auto connection: fConnections){
        // m = ;
        // Serial.println(m);
        connection->Communicate(process->Message());
      }
    }
  }

  // Get the message
  // if(fStateUpdated) for(auto process: fProcessors) process->Communicate(fESP32Config);

  // Serial.println("");


}


void ESP32TrackerManager::IMUAcquisition(){
  // if(!fSilentRunning) Serial.println("IMUAcquisition()");
  fAccelStateUpdated = false;
  fGyroStateUpdated = false;
  fMagStateUpdated = false;
  fGPSStateUpdated = false;

  accel_data_available = fESP32Config->GetAccelerometerEnable(0) && fESP32Config->GetIMU()->accelAvailable();
  gyro_data_available = fESP32Config->GetGyroscopeEnable() && fESP32Config->GetIMU()->gyroAvailable();
  mag_data_available = fESP32Config->GetMagnetometerEnable()  && fESP32Config->GetIMU()->magAvailable();
  gps_data_available = fESP32Config->GetGPSEnable()  && fESP32Config->GetGPS()->read();
  temperature_data_available = fESP32Config->GetTemperatureEnable() && fESP32Config->GetIMU()->tempAvailable();

  if(temperature_data_available){
    fESP32Config->GetIMU()->readTemp();
    temperature = fESP32Config->GetIMU()->temperature;
  } else {
    temperature = -1;
  }

  if(accel_data_available){
    fESP32Config->GetIMU()->readAccel();
    float tmpx = ax;
    float tmpy = ay;
    float tmpz = az;
    ax = fESP32Config->GetIMU()->calcAccel(fESP32Config->GetIMU()->ax);
    ay = fESP32Config->GetIMU()->calcAccel(fESP32Config->GetIMU()->ay);
    az = fESP32Config->GetIMU()->calcAccel(fESP32Config->GetIMU()->az);
    fAccelStateUpdated = !( (ax==tmpx) && (ay==tmpy) && (az==tmpz) );
  }
  if(gyro_data_available){
    fESP32Config->GetIMU()->readGyro();
    float tmpx = gx;
    float tmpy = gy;
    float tmpz = gz;
    gx = fESP32Config->GetIMU()->calcGyro(fESP32Config->GetIMU()->gx);
    gy = fESP32Config->GetIMU()->calcGyro(fESP32Config->GetIMU()->gy);
    gz = fESP32Config->GetIMU()->calcGyro(fESP32Config->GetIMU()->gz);
    fGyroStateUpdated = !( (gx==tmpx) && (gy==tmpy) && (gz==tmpz) );
  }
  if (mag_data_available){
    float tmpx = mx;
    float tmpy = my;
    float tmpz = mz;
    fESP32Config->GetIMU()->readMag();
    mx = fESP32Config->GetIMU()->calcMag(fESP32Config->GetIMU()->mx);
    my = fESP32Config->GetIMU()->calcMag(fESP32Config->GetIMU()->my);
    mz = fESP32Config->GetIMU()->calcMag(fESP32Config->GetIMU()->mz);
    fMagStateUpdated = !( (mx==tmpx) && (my==tmpy) && (mz==tmpz) );
  }
  if(gps_data_available){
    digitalWrite(13, 1);
    currentLocation = fESP32Config->GetGPS()->getGeolocation();
    float tmplat = lat;
    float tmplon = lon;
    lat = (float) currentLocation.latitude;
    lon = (float) currentLocation.longitude;
    fGPSStateUpdated = !( (lon==tmplon) && (lat==tmplat) );
  } else {
    digitalWrite(13, 0);
  }

  fState = fAccelStateUpdated
  + 2*fGyroStateUpdated
  + 4*fMagStateUpdated
  + 8*fGPSStateUpdated;

  fStateUpdated = fAccelStateUpdated || fGyroStateUpdated || fMagStateUpdated || fGPSStateUpdated;

  // if(fStateUpdated && fSensorFusionEnable) SensorFusion();

  if(!fStateUpdated){
    // Check whether we've lost touch with the data and reset if we have
    fRescueCount+=1;
    if(fRescueCount>100) Restart();
  } else {
    fRescueCount = 0;
  }

}

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
      if ((fCurrentTime - fLastTime) >= (long) fMeasurementInterval) {
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
      Serial.println("INFO: Changing to data transfer mode");
      ESP32_State = State::Transfer;
    }

    break;

    case State::Transfer:

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

    // Update the FTP connection
    // CheckFTP();
    // delay(50);

    // Check if button has been pressed
    if( (digitalRead(0) == LOW) )
    {
      while (digitalRead(0) == LOW); // Wait for button to be released
      Serial.println("INFO: Changing to data acquisition mode");

      // Turn off the LED
      if (fLEDState == HIGH) fLEDState = LOW;
      digitalWrite(5, fLEDState);

      // Disconnect from wifi if MQTT is false and WIFI is true
      // if((!fESP32Config->GetMQTTEnable()) && (fESP32Config->GetWifiEnable()))
      //     fESP32Config->DisconnectWifi();

      ESP32_State = State::Acquisition;

    }

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
