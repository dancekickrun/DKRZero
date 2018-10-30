//
//  ESP32SampleFrequency.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32SampleFrequency_hpp
#define ESP32SampleFrequency_hpp

#include <vector>
#include <stdio.h>
#include "ESP32Processor.h"

class ESP32SampleFrequency : public ESP32Processor
{

public:

    ESP32SampleFrequency(
      const char* message_format,
      const char* message_transport,
      int npoints
    );

    ~ESP32SampleFrequency();

    // Initialize the processor
    void Setup();

    //
    void ProcessData(long, float data);
    void ProcessData(long t,float la, float lo,
      float ax,float ay,float az,
      float gx,float gy,float gz,
      float mx,float my,float mz);

protected:

  int fNPoints;
  std::vector<long> tmp_data;

  float fDiffTime;

private:

};

#endif /* ESP32SampleFrequency_hpp */
