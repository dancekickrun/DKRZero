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


#include "ESP32SerialConnection.h"

ESP32SerialConnection::ESP32SerialConnection(JsonObject& json_connection)
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::Serial;

  // The data rate is sensitive to how often the data is written to the serial port
  fWriteModulo=20;
  fNumWrites=0;
  Setup(json_connection);

};



ESP32SerialConnection::~ESP32SerialConnection(){}

void ESP32SerialConnection::Setup(JsonObject& json_connection)
{

  if(json_connection.containsKey("write_every")) fWriteModulo = atoi(json_connection["write_every"]);

  Serial.print("INFO: Serial connection write every "); Serial.println(fWriteModulo);
};


void ESP32SerialConnection::Communicate(String message_input)
{

  if(fWriteModulo>1)
  {
    if((fNumWrites%fWriteModulo)==0)
    {
      Serial.print(output_data);
      output_data="";
      fNumWrites++;
    }
    else
    {
        output_data+=message_input;
        output_data+="\n";
        fNumWrites++;
    }
  } else {
    Serial.println(message_input);
  }


  // cur = millis();
  // Serial.print("Communicate FileSerial ");
  // Serial.println(cur-startMillis);

  // If bool flag is false get a new log file for every write/event seen
  // if(!fContinuousAcq){
  //   delay(50);// wait to finish file write
  //   fDataLogFile->indexNewLogFile();
  // }
};
