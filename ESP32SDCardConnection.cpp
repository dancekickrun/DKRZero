//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32SDCardConnection.h"

ESP32SDCardConnection::ESP32SDCardConnection() : fStubFilename("data"), fContinuousAcq(true)
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::SD;

};

ESP32SDCardConnection::ESP32SDCardConnection(const char* stub_filename)
: fStubFilename(stub_filename), fContinuousAcq(true)
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::SD;

};

ESP32SDCardConnection::ESP32SDCardConnection(const char* stub_filename, bool cont_acq)
: fStubFilename(stub_filename), fContinuousAcq(cont_acq)
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::SD;

};

ESP32SDCardConnection::ESP32SDCardConnection(bool cont_acq)
: fStubFilename("data"), fContinuousAcq(cont_acq)
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::SD;
};



ESP32SDCardConnection::~ESP32SDCardConnection(){}

void ESP32SDCardConnection::Initialize()
{
  Serial.println("INFO: Initializing SD Card data file connection");
  fDataLogFile = new FileSerial();
};

void ESP32SDCardConnection::Start()
{
  Serial.println("INFO: Starting SD Card data file connection");

  if(fDataLogFile->begin(&SD, 33, SPI, 10000000, "/sd") == 0)
  {
    Serial.println("ERR: Starting LogFile, write to SD did not succeed, halting.");
    while(1);
  }
  fDataLogFile->setMaxFileSize(245 + 10000*232);// 232 bytes per data point
  fDataLogFile->setWriteBufferSize(80);
  fDataLogFile->startLog("/data", fStubFilename);

};

void ESP32SDCardConnection::Stop()
{
  fDataLogFile->end();
};

void ESP32SDCardConnection::Communicate(String message_input)
{
  // Save the data to the current file
  Serial.println(message_input);
  fDataLogFile->println(message_input);

  // If bool flag is false get a new log file for every write/event seen
  // if(!fContinuousAcq){
  //   delay(50);// wait to finish file write
  //   fDataLogFile->indexNewLogFile();
  // }
};
