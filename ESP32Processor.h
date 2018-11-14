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


#ifndef ESP32Processor_h
#define ESP32Processor_h

#include <stdio.h>
#include <vector>
#include <Esp.h>
#include <ArduinoJson.h>

#include "SPIFFS.h"


class ESP32Processor {

public:

    ESP32Processor();
    ~ESP32Processor();

    // Initialize the processor
    virtual void Setup(JsonObject&)=0;

    //
    virtual void ProcessData(long, float data){};
    virtual void ProcessData(long, float , float, float, float, float, float, float, float, float, float, float){};

    void Reset(){
      MessageTransportData.clear();
      MessageTransportNames.clear();
    };
    // virtual void Communicate()=0;

    bool GetDataReady(){return fDataReady;};

    //
    String HeaderMessage();
    String Message();

    String JSONMessage()
    {
      int nelements = (int) MessageTransportNames.size();
      // Serial.println(nelements);

      // Check data and names are the same size here (TODO)

      // Prepare a JSON payload string
      String payload = "{";

      payload += "\"time\":";
      payload += "\"";
      payload += fCurrentTime;
      payload += "\", ";

      nelements=3;
      for(int i=0;i<nelements; i++)
      {
        if(i<(nelements-1))
        {
          payload += "\"";
          payload += MessageTransportNames.at(i);
          payload += "\":";
          payload += "\"";
          payload += MessageTransportData.at(i);
          payload += "\",";
        } else {
          payload += "\"";
          payload += MessageTransportNames.at(i);
          payload += "\":";
          payload += "\"";
          payload += MessageTransportData.at(i);
          payload += "\"";
        }
      }
      payload += "}";
      return payload;
    };

    enum class MessengerFormat{Space,Space_NoNames,JSON,Columns};
    enum class MessengerTransport{Serial,MQTT, SDCard};

    int GetMessageLength(){return fNelements;};
    String GetMessageDelimiter(){return fDelimiter;};

protected:

    String fOutputMessage;
    int fVerboseLevel;
    long fCurrentTime;
    MessengerFormat fMessageFormat;
    MessengerTransport fMessageTransport;
    std::vector<float> MessageTransportData;
    std::vector<String> MessageTransportNames;

    String fDelimiter;
    int fNelements;

    bool fDataReady;

  private:

    // char tmp_message[100000];// This sets a fundamental limit for the number of data elements in a buffer to be eventually printed out


};


#endif /* ESP32Processor_hpp */
