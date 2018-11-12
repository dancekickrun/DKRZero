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


#include "ESP32SDCardConnection.h"

ESP32SDCardConnection::ESP32SDCardConnection(JsonObject& json_connection) : fStubFilename("data"), fContinuousAcq(true), fWriteBufferSize(80), fMaxFileSize(245 + 10000*232)
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::SD;

  fMaxFileSize=1024*1024*100;
  fStubFilename="data";

  // The data acquisition rate is sensitive to the write buffer size
  // 10 == 6 Hz
  // 80 == 40 Hz
  // 120 == 50 Hz
  // 200 == 70 Hz
  // 200 == 70 Hz
  // 256 == 1 Hz ??
  // 300 == 25 Hz ??
  // 400 == 60
  // 800 == 20 Hz
  // ... for raw and sample frequency processors
  fWriteBufferSize=200;

  Setup(json_connection);

};



ESP32SDCardConnection::~ESP32SDCardConnection(){}

void ESP32SDCardConnection::Setup(JsonObject& json_connection)
{

  if(json_connection.containsKey("base_filename")) fStubFilename = json_connection["base_filename"];
  if(json_connection.containsKey("continuous")) fContinuousAcq = json_connection["continuous"];
  if(json_connection.containsKey("max_file_size")) fMaxFileSize = atoi(json_connection["max_file_size"]);
  if(json_connection.containsKey("write_buffer_size")) fWriteBufferSize = atoi(json_connection["write_buffer_size"]);

  // Initialize the SD connection
  Initialize();

  // Start the SD connection
  Start();

};

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
  fDataLogFile->setMaxFileSize(fMaxFileSize);
  fDataLogFile->setWriteBufferSize(fWriteBufferSize);
  fDataLogFile->startLog("/data", fStubFilename);

};

void ESP32SDCardConnection::Stop()
{
  fDataLogFile->end();
};

void ESP32SDCardConnection::Communicate(String message_input)
{

  // Save the data to the current file
  // unsigned long startMillis = millis();
  Serial.println(message_input);
  // unsigned long cur = millis();
  // Serial.print("Communicate Serial ");
  // Serial.println(cur-startMillis);


  // startMillis = millis();
  fDataLogFile->println(message_input);
  // cur = millis();
  // Serial.print("Communicate FileSerial ");
  // Serial.println(cur-startMillis);

  // If bool flag is false get a new log file for every write/event seen
  // if(!fContinuousAcq){
  //   delay(50);// wait to finish file write
  //   fDataLogFile->indexNewLogFile();
  // }
};
