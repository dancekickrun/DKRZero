//
//  ESP32Cadence.cpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32Activity.h"

ESP32Activity::ESP32Activity(const char* message_format,const char* message_transport) :
ESP32Processor(message_format, message_transport)
{

  fLowThreshold = 0.1;
  fMidThreshold = 0.5;
  fHighThreshold = 1.0;

}

ESP32Activity::~ESP32Activity() {}

void ESP32Activity::Setup(){

  ESP32Processor::MessageTransportNames.push_back(String("low_activity"));
  ESP32Processor::MessageTransportNames.push_back(String("mid_activity"));
  ESP32Processor::MessageTransportNames.push_back(String("high_activity"));

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

};

void ESP32Activity::ProcessData(long t,float la, float lo,
  float ax,float ay,float az,
  float gx,float gy,float gz,
  float mx,float my,float mz)
  {
    // Serial.println("ESP32Cadence::ProcessData()");
    ProcessData(t, sqrt(ax*ax+ay*ay+az*az));
  };

//
void ESP32Activity::ProcessData(long CurrentTime, float data_point)
{
  data_point -=1.0;
  data_point = abs(data_point);

  bool low_activity = (data_point>fLowThreshold)&&(data_point<fMidThreshold);
  bool mid_activity = (data_point>fMidThreshold)&&(data_point<fHighThreshold);
  bool hi_activity = (data_point>fHighThreshold);

  ESP32Processor::MessageTransportData[0] = low_activity ? 1.0 : 0.0;
  ESP32Processor::MessageTransportData[1] = mid_activity ? 1.0 : 0.0;
  ESP32Processor::MessageTransportData[2] = hi_activity ? 1.0 : 0.0;

};
