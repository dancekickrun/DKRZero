//
//  ESP32SensorFusion.cpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//
#include "ESP32SensorFusion.h"

ESP32SensorFusion::ESP32SensorFusion(bool UseMahony, bool UseMag, const char* message_format,const char* message_transport) :
 ESP32Processor(message_format, message_transport)
{

  fPitch = 30.0*DEG_TO_RAD;
  fRoll = 30.0*DEG_TO_RAD;
  fYaw = 30.0*DEG_TO_RAD;

  fMahony = UseMahony;
  fMagEnable = UseMag;

  deltat = filter.deltatUpdate();

}

void ESP32SensorFusion::Setup()
{
  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back("roll");
  ESP32Processor::MessageTransportNames.push_back("pitch");
  ESP32Processor::MessageTransportNames.push_back("yaw");

  // ESP32Processor::MessageTransport = ESP32Processor::MessengerFormat::Space;

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);
}


ESP32SensorFusion::~ESP32SensorFusion() {}

void ESP32SensorFusion::ProcessData(long CurrentTime, float la, float lo, float ax,float ay,float az, float gx,float gy,float gz, float mx,float my,float mz)
{
  ESP32Processor::fCurrentTime = CurrentTime;
  // Serial.println("ESP32SensorFusion::ProcessData()");

  float deltat = filter.deltatUpdate(); //this have to be done before calling the filter update
  filter.MahonyUpdate(gx * DEG_TO_RAD, gy * DEG_TO_RAD, gz * DEG_TO_RAD, ax, ay, az, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow

  // if(fMagEnable && fMahony){
  //   float deltat = filter.deltatUpdate(); //this have to be done before calling the filter update
  //   filter.MahonyUpdate(gx * DEG_TO_RAD, gy * DEG_TO_RAD, gz * DEG_TO_RAD, ax, ay, az, mx,my,mz, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow
  // }
  //
  // if((!fMagEnable) && fMahony){
  //   float deltat = filter.deltatUpdate(); //this have to be done before calling the filter update
  //   filter.MahonyUpdate(gx * DEG_TO_RAD, gy * DEG_TO_RAD, gz * DEG_TO_RAD, ax, ay, az, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow
  // }
  //
  // if(fMagEnable && (!fMahony)){
  //   float deltat = filter.deltatUpdate(); //this have to be done before calling the filter update
  //   filter.MadgwickUpdate(gx * DEG_TO_RAD, gy * DEG_TO_RAD, gz * DEG_TO_RAD, ax, ay, az, mx,my,mz, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow
  // }
  //
  // if((!fMagEnable) && (!fMahony)){
  //   float deltat = filter.deltatUpdate(); //this have to be done before calling the filter update
  //   filter.MadgwickUpdate(gx * DEG_TO_RAD, gy * DEG_TO_RAD, gz * DEG_TO_RAD, ax, ay, az, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow
  // }

  // Serial.println(Message());

  ESP32Processor::MessageTransportData.clear();
  ESP32Processor::MessageTransportData.push_back(filter.getRoll());
  ESP32Processor::MessageTransportData.push_back(filter.getPitch());
  ESP32Processor::MessageTransportData.push_back(filter.getYaw());

  return;
};
