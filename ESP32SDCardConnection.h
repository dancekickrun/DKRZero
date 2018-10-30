//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32SDCardConnection_hpp
#define ESP32SDCardConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>
#include "ESP32Connection.h"
#include "FileSerial.h"

// SD Card connections class - assumes that the SD card is already mounted to read the config.txt file
class ESP32SDCardConnection : public ESP32Connection {

public:

    ESP32SDCardConnection();
    ESP32SDCardConnection(const char* stub_filename);
    ESP32SDCardConnection(const char* stub_filename, bool cont_acq);
    ESP32SDCardConnection(bool cont_acq);

    ~ESP32SDCardConnection();

    void Initialize();
    void Start();
    void Stop();
    void Communicate(String);


private:

  FileSerial* fDataLogFile;
  const char* fStubFilename;
  bool fContinuousAcq;// if true then don't get a new file for every event

};


#endif
