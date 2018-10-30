//
//  ESP32Configuration.cpp
//  trigger
//
//  Created by Chris Steer on 09/08/2018.
//
//

#include "ESP32Configuration.h"




ESP32Configuration::ESP32Configuration() : Serial1(2), gps(Serial1)
{


  fSilentRunning = false;
  // fDataLogFile = new FileSerial(&Serial);
  fSaveInfoLog = false;
  fInfoLogFile = new FileSerial(&Serial);
  // ftpSrv = new FtpServer();

};

void ESP32Configuration::Configure(JsonObject& root)
{

  // Configure the sensors
  SetupSensors(root);

  // // Check wifi and mqtt (if mqtt is enabled then enable wifi too)
  // fMQTTEnable = (bool) root["mqtt_enable"];
  // Serial.print("INFO: MQQTEnable ");
  // Serial.println(fMQTTEnable);
  //
  // if(!fMQTTEnable){// if MQTT is not enabled check whether the user would like wifi enabled
  //   fWifiEnable = (bool) root["wifi_enable"];
  // } else {
  //   // MQTTenable is true so make wifienable also true
  //   fWifiEnable = fMQTTEnable;
  // }
  // Serial.print("INFO: WiFiEnable ");
  // Serial.println(fWifiEnable);
  //
  // if(!fWifiEnable){
  //   Serial.println("INFO: Wifi disabled (\"wifi_enable\" : 0 ) ");
  // } else {
  //   // Then setup wifi
  //   SetupWifi(root);
  //   ConnectWifi();
  //   // Only setup the ftp server if the wifi is also setup
  //   FTPSetup();
  //
  //   // If MQTT is enabled then we need to connect wifi here:
  //   if(fMQTTEnable){
  //     Serial.println("INFO: Setting up MQTT");
  //     SetupMQTT(root);
  //   }
  // }


  // SetupWriteData(root);

};




// void ESP32Configuration::FTPSetup()
// {
//
//   // Open the Flash File System first for FTP Server
//   // if (SPIFFS.begin(true)) {
//   // Serial.println("INFO: Opened SPIFFS ");
//   uint8_t cardType = SD.cardType();
//   if(cardType != CARD_NONE){
//     if(!fSilentRunning) Serial.println("INFO: SD Card Found for FTP");
//     // fInfoFile.println("INFO: SD Card Found for FTP");
//
//     ftpSrv->begin("esp32","esp32"); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
//   } else {
//     if(!fSilentRunning) Serial.println("INFO: Failed to open SD Card for ftp server ");
//     // fInfoFile.println("INFO: Failed to open SD Card for ftp server ");
//   }
//
// }



void ESP32Configuration::SetupSDCard()
{

  //Call begin with (cs pin, SPI, rate (up to 10MHz), "/sd")
  // Mount SD Card
  if(!SD.begin(33, SPI, 10000000, "/sd")){
    if(!fSilentRunning) Serial.println("Card Mount Failed");

    // Setup error codes:
    //
    // # Fast Flashes
    //    3                  Cannot start IMU
    //    4                  Cannot mount SD
    while (1){Indicator(4,100,100,1000);};
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


void ESP32Configuration::SetupSensors(JsonObject &root){

  // Get the desired data rate (if -1 then free running)
  if(root.containsKey("desired_data_rate")){
    fDesiredDataRate = (float) root["desired_data_rate"];
  } else {
    fDesiredDataRate = -1;
  }
  // Need to check if this json field exists first?
  Serial.print("INFO: Desired Sample Rate is "); Serial.println(fDesiredDataRate);


  SetupGyroscope(root);
  SetupAccelerometer(root);
  SetupMagnetometer(root);
  SetupGPS(root);
  SetupTemperature(root);

  // SetupMessenger(root);
}


void ESP32Configuration::Indicator(int nflash, int led_on_ms=250, int led_off_ms=250,int led_end_pause=1000){

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





// void ESP32Configuration::SetupMQTT(JsonObject &root){
//
//   if(!fMQTTEnable){
//     Serial.println("INFO: MQTT disabled");
//     return;
//   }
//
//   fMQTTServerIP = (const char*) root["mqtt_server"];
//   fMQTTServerPort = (const int) root["mqtt_port"];// 1883
//   fMQTTThingName = (const char*) root["mqtt_thing_name"];
//
//
//   // const char* esp_thing_name = "ESP-001";
//   fMQTTAccessToken = (const char*) root["mqtt_thing_access_token"];
//
//   Serial.println(fMQTTServerIP);
//   Serial.println(fMQTTServerPort);
//   Serial.println(fMQTTThingName);
//   Serial.println(fMQTTAccessToken);
//   // const char* esp_thing_access_token = "covXcXbkZQQAdSdzQ6zo";
//
//   // Connect to the Thingsboard
//   if (WiFi.status() == WL_CONNECTED) {
//
//     while (!fMQTTClient.connected()) {
//
//       Serial.print("Connecting to Thingsboard node ...");
//       fPubSubClient.setServer(fMQTTServerIP,fMQTTServerPort);
//
//       if ( fPubSubClient.connect(fMQTTThingName, fMQTTAccessToken, NULL) ) {
//         Serial.println( "[DONE]" );
//       } else {
//         Serial.print( "[FAILED] [ rc = " );
//         Serial.print( fPubSubClient.state() );
//         Serial.println( " : retrying in 1 second]" );
//         // Wait 1 second before retrying
//         delay( 1000 );
//       }
//
//     }// else need to connect wifi
//
//   }// End While
//
// }






// void ESP32Configuration::SetupWriteData(JsonObject &root){
//
//   if(fSaveInfoLog){
//     //call begin with device, CS pin, port, frequency, and mount point.
//     if(fInfoLogFile->begin(&SD, 33, SPI, 10000000, "/sd") == 0)
//     {
//       if(!fSilentRunning) Serial.println("ERR: Starting InfoFile, write to SD did not succeed, halting.");
//     }
//     fInfoLogFile->setMaxFileSize(245 + 10000*232);// 232 bytes per data point
//     fInfoLogFile->setWriteBufferSize(240);
//     fInfoLogFile->startLog("/", "info_");
//   }
//
//   fFileWriteEnable = (bool) root["write_file_enable"];
//
//   // Need to check the file write
//
//   if(fFileWriteEnable){
//     if(!fSilentRunning) Serial.println("INFO: Writing IMU data to file enabled");
//     // fInfoFile.println("INFO: Changing to data acquisition mode");
//
//     //call begin with device, CS pin, port, frequency, and mount point.
//     if(fDataLogFile->begin(&SD, 33, SPI, 10000000, "/sd") == 0)
//     {
//
//       if(!fSilentRunning) Serial.println("ERR: Starting LogFile, write to SD did not succeed, halting.");
//       // fInfoFile.println("ERR: Starting LogFile, write to SD did not succeed, halting.");
//       while(1);
//
//     }
//
//     fDataLogFile->setMaxFileSize(245 + 10000*232);// 232 bytes per data point
//     fDataLogFile->setWriteBufferSize(80);
//     fDataLogFile->startLog("/", "trak_");
//
//   } else{
//     // Save debug data to a file
//
//     if(!fSilentRunning) Serial.println("INFO: Writing data to file disabled");
//     fDataLogFile->println("INFO: Writing data to file disabled");
//
//   }
//
//
//
// }


void ESP32Configuration::SetupGyroscope(JsonObject &root){

  if(!fSilentRunning) Serial.println("INFO: Configuring Gyroscope");
  if(fSaveInfoLog) fInfoLogFile->println("INFO: Configuring Gyroscope");

  // [enabled] turns the gyro on or off.
  fGyroEnable = (bool) root["gyro_enable"];
  imu.settings.gyro.enabled = fGyroEnable;  // Enable the gyro

  // [scale] sets the full-scale range of the gyroscope.
  // scale can be set to either 245, 500, or 2000
  // imu.settings.gyro.scale = 245; // Set scale to +/-245dps
  fGyroScale = root["gyro_scale"];
  // Round to one of the three
  if(fGyroScale<245) fGyroScale = 245;
  if((fGyroScale>245)&&(fGyroScale<372)) fGyroScale = 245;
  if((fGyroScale>372)&&(fGyroScale<500)) fGyroScale = 500;
  if(fGyroScale>500) fGyroScale = 500;
  imu.settings.gyro.scale = fGyroScale; // Set scale to +/-245dps



  // [sampleRate] sets the output data rate (ODR) of the gyro
  // sampleRate can be set between 1-6
  // 1 = 14.9    4 = 238
  // 2 = 59.5    5 = 476
  // 3 = 119     6 = 952
  fGyroSampleRate = root["gyro_sample_rate"];
  if(fGyroSampleRate<1) fGyroSampleRate = 1;
  if(fGyroSampleRate>6) fGyroSampleRate = 6;
  imu.settings.gyro.sampleRate = fGyroSampleRate; // 59.5Hz ODR

  // [bandwidth] can set the cutoff frequency of the gyro.
  // Allowed values: 0-3. Actual value of cutoff frequency
  // depends on the sample rate. (Datasheet section 7.12)
  fGyroBandwidth = root["gyro_bandwidth"];
  // imu.settings.gyro.bandwidth = 0;
  if(fGyroBandwidth<0) fGyroBandwidth = 0;
  if(fGyroBandwidth>3) fGyroBandwidth = 3;
  imu.settings.gyro.bandwidth = fGyroBandwidth;

  // [lowPowerEnable] turns low-power mode on or off.
  fGyroLowPowerEnable = (bool) root["gyro_low_power_enable"];
  imu.settings.gyro.lowPowerEnable = fGyroLowPowerEnable; // LP mode off

  fGyroHPFDisable = (bool) root["gyro_high_pass_filter_disable"];
  // [HPFEnable] enables or disables the high-pass filter
  // imu.settings.gyro.HPFEnable = true; // HPF disabled
  imu.settings.gyro.HPFEnable = fGyroHPFDisable; // HPF disabled

  // [HPFCutoff] sets the HPF cutoff frequency (if enabled)
  // Allowable values are 0-9. Value depends on ODR.
  // (Datasheet section 7.14)
  fGyroHPFCutOff = root["gyro_high_pass_filter_cutoff"];
  if(fGyroHPFCutOff>9) fGyroHPFCutOff = 9;
  if(fGyroHPFCutOff<0) fGyroHPFCutOff = 0;
  // imu.settings.gyro.HPFCutoff = 1; // HPF cutoff = 4Hz
  imu.settings.gyro.HPFCutoff = fGyroHPFCutOff; // HPF cutoff = 4Hz

  // [flipX], [flipY], and [flipZ] are booleans that can
  // automatically switch the positive/negative orientation
  // of the three gyro axes.
  fGyroFlipX = (bool) root["gyro_flip_x"];
  fGyroFlipY = (bool) root["gyro_flip_y"];
  fGyroFlipZ = (bool) root["gyro_flip_z"];
  // imu.settings.gyro.flipX = false; // Don't flip X
  // imu.settings.gyro.flipY = false; // Don't flip Y
  // imu.settings.gyro.flipZ = false; // Don't flip Z
  imu.settings.gyro.flipX = fGyroFlipX; // Don't flip X
  imu.settings.gyro.flipY = fGyroFlipY; // Don't flip Y
  imu.settings.gyro.flipZ = fGyroFlipZ; // Don't flip Z

};

void ESP32Configuration::SetupAccelerometer(JsonObject &root){

  if(!fSilentRunning) Serial.println("INFO: Configuring Accelerometer");
  fInfoLogFile->println("INFO: Configuring Accelerometer");

  /**
  *
  * Setup IMU from JSON
  *
  */

  // [scale] sets the full-scale range of the accelerometer.
  // accel scale can be 2, 4, 8, or 16
  unsigned int v = (unsigned int) root["acceleration_scale"];
  // Round this to the nearest power of 2
  v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++;
  fAccelScale = (int) v;
  if(fAccelScale>16) fAccelScale = 16;
  if(fAccelScale<1) fAccelScale = 1;
  imu.settings.accel.scale = fAccelScale; // Set accel scale to +/-8g.

  fAccelEnable = (bool) root["acceleration_enable"];
  fAccelXEnable = (bool) root["acceleration_x_enable"];
  fAccelYEnable = (bool) root["acceleration_y_enable"];
  fAccelZEnable = (bool) root["acceleration_z_enable"];
  imu.settings.accel.enabled = fAccelEnable; // Enable accelerometer
  imu.settings.accel.enableX = fAccelXEnable; // Enable X
  imu.settings.accel.enableY = fAccelYEnable; // Enable Y
  imu.settings.accel.enableZ = fAccelZEnable; // Enable Z

  // [sampleRate] sets the output data rate (ODR) of the
  // accelerometer. ONLY APPLICABLE WHEN THE GYROSCOPE IS
  // DISABLED! Otherwise accel sample rate = gyro sample rate.
  // accel sample rate can be 1-6
  // 1 = 10 Hz    4 = 238 Hz
  // 2 = 50 Hz    5 = 476 Hz
  // 3 = 119 Hz   6 = 952 Hz
  fAccelSampleRate = atoi(root["acceleration_sample_rate"]);
  Serial.print("INFO: Setting sample rate control to be ");Serial.println(fAccelSampleRate);
  if(fAccelSampleRate>6) fAccelSampleRate = 6;
  if(fAccelSampleRate<1) fAccelSampleRate = 1;
  imu.settings.accel.sampleRate = fAccelSampleRate; // Set acceleration sample rate


  // [bandwidth] sets the anti-aliasing filter bandwidth.
  // Accel cutoff freqeuncy can be any value between -1 - 3.
  // -1 = bandwidth determined by sample rate
  // 0 = 408 Hz   2 = 105 Hz
  // 1 = 211 Hz   3 = 50 Hz
  fAccelBandwidth = root["acceleration_bandwidth"];
  if(fAccelSampleRate>3) fAccelSampleRate = 3;
  if(fAccelSampleRate<-1) fAccelSampleRate = -1;
  imu.settings.accel.bandwidth = fAccelBandwidth; // BW = 408Hz

  // [highResEnable] enables or disables high resolution
  // mode for the acclerometer.
  fAccelHighResEnable = (bool) root["acceleration_high_resolution_enable"];
  imu.settings.accel.highResEnable = fAccelHighResEnable; // Disable HR

  // [highResBandwidth] sets the LP cutoff frequency of
  // the accelerometer if it's in high-res mode.
  // can be any value between 0-3
  // LP cutoff is set to a factor of sample rate
  // 0 = ODR/50    2 = ODR/9
  // 1 = ODR/100   3 = ODR/400
  fAccelHighResBandwidth = root["acceleration_high_resolution_bandwidth"];
  if(fAccelHighResBandwidth>3) fAccelHighResBandwidth = 3;
  if(fAccelHighResBandwidth<0) fAccelHighResBandwidth = 0;
  imu.settings.accel.highResBandwidth = fAccelHighResBandwidth;

};



void ESP32Configuration::SetupMagnetometer(JsonObject &root){

  if(!fSilentRunning) Serial.println("INFO: Configuring Magnetometer");
  if(fSaveInfoLog) fInfoLogFile->println("INFO: Configuring Magnetometer");



  // [enabled] turns the magnetometer on or off.
  fMagEnable = (bool) root["magnetometer_enable"];
  imu.settings.mag.enabled = fMagEnable; // Enable magnetometer

  // [scale] sets the full-scale range of the magnetometer
  // mag scale can be 4, 8, 12, or 16
  fMagScale = root["magnetometer_scale"];
  // imu.settings.mag.scale = 12; // Set mag scale to +/-12 Gs
  if(fMagScale<4) {
    fMagScale = 4;
  } else if((fMagScale > 4)&&(fMagScale<6)){
    fMagScale = 6;
  } else if((fMagScale > 6)&&(fMagScale<10)){
    fMagScale = 8;
  } else if((fMagScale > 10)&&(fMagScale<14)){
    fMagScale = 12;
  } else if(fMagScale > 14){
    fMagScale = 16;
  }
  imu.settings.mag.scale = fMagScale; // Set mag scale to +/-12 Gs



  // [sampleRate] sets the output data rate (ODR) of the
  // magnetometer.
  // mag data rate can be 0-7:
  // 0 = 0.625 Hz  4 = 10 Hz
  // 1 = 1.25 Hz   5 = 20 Hz
  // 2 = 2.5 Hz    6 = 40 Hz
  // 3 = 5 Hz      7 = 80 Hz
  // imu.settings.mag.sampleRate = 5; // Set OD rate to 20Hz
  fMagSampleRate = root["magnetometer_sample_rate"];
  if(fMagSampleRate<0) fMagSampleRate = 0;
  if(fMagSampleRate>7) fMagSampleRate = 7;
  imu.settings.mag.sampleRate = fMagSampleRate; // Set OD rate to 20Hz

  // [tempCompensationEnable] enables or disables
  // temperature compensation of the magnetometer.
  // imu.settings.mag.tempCompensationEnable = false;
  fMagTempCompEnable = (bool) root["magnetometer_temperature_compensation_enable"];
  imu.settings.mag.tempCompensationEnable = fMagTempCompEnable;

  // [XYPerformance] sets the x and y-axis performance of the
  // magnetometer to either:
  // 0 = Low power mode      2 = high performance
  // 1 = medium performance  3 = ultra-high performance
  // imu.settings.mag.XYPerformance = 3; // Ultra-high perform.
  fMagXYPerformance = root["magnetometer_xy_performance"];
  imu.settings.mag.XYPerformance = fMagXYPerformance; // Ultra-high perform.
  if(fMagXYPerformance<0) fMagXYPerformance = 0;
  if(fMagXYPerformance>3) fMagXYPerformance = 3;

  // [ZPerformance] does the same thing, but only for the z
  // imu.settings.mag.ZPerformance = 3; // Ultra-high perform.
  fMagZPerformance = root["magnetometer_z_performance"];
  if(fMagZPerformance<0) fMagZPerformance = 0;
  if(fMagZPerformance>3) fMagZPerformance = 3;
  imu.settings.mag.ZPerformance = fMagZPerformance; // Ultra-high perform.


  // [lowPowerEnable] enables or disables low power mode in
  // the magnetometer.
  // imu.settings.mag.lowPowerEnable = false;
  fMagLowPowerEnable = (bool) root["magnetometer_low_power_enable"];
  imu.settings.mag.lowPowerEnable = fMagLowPowerEnable;




  // [operatingMode] sets the operating mode of the
  // magnetometer. operatingMode can be 0-2:
  // 0 = continuous conversion
  // 1 = single-conversion
  // 2 = power down
  fMagOperationMode = root["magnetometer_operation_mode"];
  // imu.settings.mag.operatingMode = 0; // Continuous mode
  if(fMagOperationMode<0) fMagOperationMode = 0;
  if(fMagOperationMode>2) fMagOperationMode = 2;
  imu.settings.mag.operatingMode = fMagOperationMode; // Continuous mode

}

void ESP32Configuration::SetupTemperature(JsonObject &root){

  if(!fSilentRunning) Serial.println("INFO: Configuring Temperature Sensor");
  if(fSaveInfoLog) fInfoLogFile->println("INFO: Configuring Temperature Sensor");


  // [enabled] turns the temperature sensor on or off.
  // imu.settings.temp.enabled = true;
  // imu.settings.mag.operatingMode = 0; // Continuous mode
  fTemperatureEnable = (bool) root["temperature_enable"];
  imu.settings.temp.enabled = fTemperatureEnable;
}

// void ESP32Configuration::SetupWifi(JsonObject &root){
//   /**
//   * Something about setting up the recording format here
//   */
//   if(!fWifiEnable) return;
//
//   Serial.println("INFO: Configuring Wifi Connection");
//
//   // Parse the contents and setup the uC
//   fWifiNetworkName = root["wifi_network_name"];
//   fWifiNetworkPassword = root["wifi_network_password"];
//
//   // Save the IP address
//   Serial.print("INFO: IP address: ");
//   fWifiIPAddress = WiFi.localIP().toString();
//   Serial.println(fWifiIPAddress);
//   fWifiStatus = WL_CONNECTED;
//
// }
//
// void ESP32Configuration::DisconnectWifi(){
//
//   // Stop the connection
//   WiFi.disconnect();
//   while (WiFi.status() != WL_DISCONNECTED){
//     digitalWrite(5, 1);
//     delay(500);
//     Serial.print(".");
//     digitalWrite(5, 0);
//   }
//   Serial.println("");
//   Serial.println("INFO: WiFi disconnected");
//
// }
//
// void ESP32Configuration::ConnectWifi(){
//
//   if(WiFi.status() == WL_CONNECTED) return;
//
//   Serial.print("INFO: Wifi connecting to ");
//   Serial.println(fWifiNetworkName);
//
//   WiFi.begin(fWifiNetworkName, fWifiNetworkPassword);
//   while (WiFi.status() != WL_CONNECTED){
//     digitalWrite(5, 1);
//     delay(500);
//     Serial.print(".");
//     digitalWrite(5, 0);
//   }
//   Serial.println("");
//   Serial.println("INFO: WiFi connected");
//   Serial.print("INFO: IP address: ");
//   fWifiIPAddress = WiFi.localIP().toString();
//   Serial.println(fWifiIPAddress);
//
// }


void ESP32Configuration::Print(){
  String line_break = "---------------------------------------------";
  Serial.println(line_break);

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

  String s;
  s = "acceleration_scale: "; s+=fAccelScale; Serial.println(s);
  s = "acceleration_enable: "; s+=fAccelEnable; Serial.println(s);
  s = "acceleration_x_enable: "; s+=fAccelXEnable; Serial.println(s);
  s = "acceleration_y_enable: "; s+=fAccelYEnable; Serial.println(s);
  s = "acceleration_z_enable: "; s+=fAccelZEnable; Serial.println(s);
  s = "acceleration_bandwidth: "; s+=fAccelBandwidth; Serial.println(s);
  s = "acceleration_sample_rate: "; s+=fAccelBandwidth; Serial.println(s);
  s = "acceleration_high_resolution_enable: "; s+=fAccelHighResEnable; Serial.println(s);
  s = "acceleration_high_resolution_bandwidth: "; s+=fAccelHighResBandwidth; Serial.println(s);
  s = "---------------------------------------------"; Serial.println(s);
  s = "gyro_scale: "; s+=fGyroScale; Serial.println(s);
  s = "gyro_enable: "; s+=fGyroEnable; Serial.println(s);
  s = "gyro_bandwidth: "; s+=fGyroBandwidth; Serial.println(s);
  s = "gyro_flip_x: "; s+=fGyroFlipX; Serial.println(s);
  s = "gyro_flip_y: "; s+=fGyroFlipY; Serial.println(s);
  s = "gyro_flip_z: "; s+=fGyroFlipZ; Serial.println(s);
  s = "gyro_sample_rate: "; s+=fGyroSampleRate; Serial.println(s);
  s = "gyro_low_power_enable: "; s+=fGyroLowPowerEnable; Serial.println(s);
  s = "gyro_high_pass_filter_cutoff: "; s+=fGyroHPFCutOff; Serial.println(s);
  s = "gyro_high_pass_filter_disable: "; s+=fGyroHPFDisable; Serial.println(s);
  s = "---------------------------------------------"; Serial.println(s);
  s = "magnetometer_scale: "; s+=fMagScale; Serial.println(s);
  s = "magnetometer_enable: "; s+=fMagEnable; Serial.println(s);
  s = "magnetometer_sample_rate: "; s+=fMagSampleRate; Serial.println(s);
  s = "magnetometer_xy_performance: "; s+=fMagXYPerformance; Serial.println(s);
  s = "magnetometer_z_performance: "; s+=fMagZPerformance; Serial.println(s);
  s = "magnetometer_operation_mode: "; s+=fMagOperationMode; Serial.println(s);
  s = "magnetometer_low_power_enable: "; s+=fMagLowPowerEnable; Serial.println(s);
  s = "magnetometer_temperature_compensation_enable: "; s+=fMagTempCompEnable; Serial.println(s);
  s = "---------------------------------------------"; Serial.println(s);
  s = "temperature_enable: "; s+=fTemperatureEnable; Serial.println(s);
  s = "---------------------------------------------"; Serial.println(s);
  // s = "wifi_enable: "; s+=fWifiEnable; Serial.println(s);
  // s = "wifi_network_name: "; s+=fWifiNetworkName; Serial.println(s);
  // s = "wifi_network_password: "; s+=fWifiNetworkPassword; Serial.println(s);
  // s = "wifi_ip_address: "; s+=fWifiIPAddress; Serial.println(s);
  // s = "wifi_status: "; s+=fWifiStatus; Serial.println(s);
  // s = "---------------------------------------------"; Serial.println(s);
  s = "gps_enable: "; s+=fGPSEnable; Serial.println(s);
  s = "---------------------------------------------"; Serial.println(s);
  s = "write_file_enable: "; s+=fFileWriteEnable; Serial.println(s);
  s = "desired_data_rate: "; s+=fDesiredDataRate; Serial.println(s);
  // s = "message_output: "; s+=fDesiredDataRate; Serial.println(s);
  // s = "message_output_form: "; s+=fDesiredDataRate; Serial.println(s);

  Serial.println("");

}; // End of Print()


void ESP32Configuration::SetupGPS(JsonObject &root){
  // Initialize the GPS here
  fGPSEnable =  (bool) root["gps_enable"];

  if(fGPSEnable){
    if(!fSilentRunning) Serial.println("INFO: GPS Enabled ");
    // fInfoFile.println("INFO: GPS Enabled ");
  }

  if(!fGPSEnable){
    if(!fSilentRunning) Serial.println("INFO: GPS Disabled ");
    // fInfoFile.println("INFO: GPS Disabled ");
  }

  // Start the GPS module
  gps.begin();

}
