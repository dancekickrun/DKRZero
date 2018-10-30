//
//  ESP32Processor.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32Processor_h
#define ESP32Processor_h

#include <stdio.h>
#include <vector>
#include <ESP32Configuration.h>
#include <Esp.h>
#include "SPIFFS.h"

class ESP32Configuration;

class ESP32Processor {

public:

    ESP32Processor(const char* , const char*);
    ~ESP32Processor();

    // Initialize the processor
    virtual void Setup()=0;

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

};


#endif /* ESP32Processor_hpp */
