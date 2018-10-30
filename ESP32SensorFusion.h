//
//  SensorFusion.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32SensorFusion_hpp
#define ESP32SensorFusion_hpp

#include "ESP32Processor.h"
#include "ESP32SensorFusion.h" //SF
#include "SensorFusion.h" //SF
#include <stdio.h>

class ESP32SensorFusion : public ESP32Processor {

public:

    //
    ESP32SensorFusion(bool,bool, const char* message_format,const char* message_transport);
    ~ESP32SensorFusion();

    // Initialize the processor
    void Setup();

    String Message();

    //
    void ProcessData(long, float la, float lo, float ax,float ay,float az, float gx,float gy,float gz, float mx,float my,float mz);

protected:

  bool fMahony;
  bool fMagEnable;
  float fPitch, fRoll, fYaw;

  // For the Sensor Fusion
  SF filter;

  float deltat;

 private:

};


#endif /* SensorFusion_hpp */
