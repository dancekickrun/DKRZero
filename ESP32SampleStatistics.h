//
//  ESP32SampleStatistics.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32SampleStatistics_hpp
#define ESP32SampleStatistics_hpp

#include <vector>
#include <stdio.h>
#include "ESP32Processor.h"

class ESP32SampleStatistics : public ESP32Processor
{

public:

    ESP32SampleStatistics(
      const char* message_format,
      const char* message_transport,
      int npoints,
      std::vector<bool> onoff
    );

    ~ESP32SampleStatistics();

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
  std::vector<bool> onoffs;

  // Vectors to hold the last N points for 9 measurements
  std::vector< std::vector<float> > measurements;

private:

};

#endif /* ESP32SampleStatistics_hpp */
