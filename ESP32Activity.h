//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32Activity_hpp
#define ESP32Activity_hpp

#include <vector>
#include <stdio.h>
#include "ESP32Processor.h"
#include "arduinoFFT.h"

class ESP32Activity : public ESP32Processor {
public:
    ESP32Activity(const char* message_format,const char* message_transport);
    ~ESP32Activity();

    // Initialize the processor
    void Setup();

    //
    void ProcessData(long, float data);
    void ProcessData(long t,float la, float lo,
      float ax,float ay,float az,
      float gx,float gy,float gz,
      float mx,float my,float mz);

    void SetLowThreshold(float thresh){fLowThreshold = thresh;};
    void SetMidThreshold(float thresh){fMidThreshold = thresh;};
    void SetHighThreshold(float thresh){fHighThreshold = thresh;};

private:

  float fLowThreshold;
  float fMidThreshold;
  float fHighThreshold;

};


#endif /* ESP32Cadence_hpp */
