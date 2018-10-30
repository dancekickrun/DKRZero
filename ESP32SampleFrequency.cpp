//
//  ESP32SampleFrequency.cpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32SampleFrequency.h"

ESP32SampleFrequency::ESP32SampleFrequency(const char* message_format, const char* message_transport, int npoints):
ESP32Processor(message_format, message_transport), fNPoints(npoints)
{}

ESP32SampleFrequency::~ESP32SampleFrequency()
{

}

void ESP32SampleFrequency::Setup()
{
  Serial.println("INFO: Setting up ESP32SampleFrequency ");
  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back("sample_frequency");

  for(int i=0;i<fNPoints;i++)
  {
    tmp_data.push_back((long) 0);
  }
};



void ESP32SampleFrequency::ProcessData(long t,float la, float lo,
  float ax,float ay,float az,
  float gx,float gy,float gz,
  float mx,float my,float mz)
  {
    ESP32Processor::MessageTransportData.clear();
    ESP32Processor::MessageTransportData.push_back(t);

    // remove the first element
    tmp_data.erase(tmp_data.begin());
    // add to the last element
    tmp_data.push_back(t);

    // Calculate the average time difference
    fDiffTime=0;
    for(int j=0;j<tmp_data.size()-1;j++) fDiffTime += (float) (tmp_data.at(j+1)-tmp_data.at(j));
    fDiffTime /= ((float) (tmp_data.size()-1));

    ESP32Processor::MessageTransportData.push_back(((float) 1000.0)/((float) fDiffTime));

  };

void ESP32SampleFrequency::ProcessData(long CurrentTime, float data_point){};
