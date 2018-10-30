//
//  ESP32RawData.cpp
//  trigger
//
//  Created by Chris Steer on 14/08/2018.
//
//

#include "ESP32RawData.h"

ESP32RawData::ESP32RawData(const char* message_format,const char* message_transport) :
  ESP32Processor(message_format,message_transport)
{
  ESP32Processor::fDataReady = true;
};

ESP32RawData::~ESP32RawData(){};

// Initialize the processor
void ESP32RawData::Setup()
{
  fDelimiter = String(" ");

  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back(String("time"));
  ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_x"));
  ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_y"));
  ESP32Processor::MessageTransportNames.push_back(String("linear_acceleration_z"));

  ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_x"));
  ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_y"));
  ESP32Processor::MessageTransportNames.push_back(String("angular_acceleration_z"));

  ESP32Processor::MessageTransportNames.push_back(String("magnetometer_x"));
  ESP32Processor::MessageTransportNames.push_back(String("magnetometer_y"));
  ESP32Processor::MessageTransportNames.push_back(String("magnetometer_z"));

  ESP32Processor::MessageTransportNames.push_back(String("latitude"));
  ESP32Processor::MessageTransportNames.push_back(String("longitude"));

  // ESP32Processor::MessageTransport = ESP32Processor::MessengerFormat::Space;

  ESP32Processor::MessageTransportData.clear();
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
};


//
void ESP32RawData::ProcessData(long, float data){};
void ESP32RawData::ProcessData(long t, float lat, float lon,
  float aax, float aay, float aaz,
  float ggx, float ggy, float ggz,
  float mmx, float mmy, float mmz)
  {
    // Serial.println("ESP32RawData::ProcessData()");

    ESP32Processor::fCurrentTime = t;
    ax=aax;ay=aay;az=aaz;
    gx=ggx;gy=ggy;gz=ggz;
    mx=mmx;my=mmy;mz=mmz;
    fLatitude = lat;
    fLongitude = lon;

    ESP32Processor::MessageTransportData.clear();
    ESP32Processor::MessageTransportData.push_back((float) t);

    ESP32Processor::MessageTransportData.push_back(ax);
    ESP32Processor::MessageTransportData.push_back(ay);
    ESP32Processor::MessageTransportData.push_back(az);

    ESP32Processor::MessageTransportData.push_back(gx);
    ESP32Processor::MessageTransportData.push_back(gy);
    ESP32Processor::MessageTransportData.push_back(gz);

    ESP32Processor::MessageTransportData.push_back(mx);
    ESP32Processor::MessageTransportData.push_back(my);
    ESP32Processor::MessageTransportData.push_back(mz);

    ESP32Processor::MessageTransportData.push_back(fLatitude);
    ESP32Processor::MessageTransportData.push_back(fLongitude);

  };
