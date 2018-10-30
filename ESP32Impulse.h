//
//  ESP32Impulse.hpp
//  trigger
//
//  Created by Chris Steer on 13/08/2018.
//
//

#ifndef ESP32Impulse_h
#define ESP32Impulse_h

#include <stdio.h>
#include "ESP32Processor.h"
#include <math.h>
#include <queue>

class ESP32Impulse : public ESP32Processor
{

public:

    ESP32Impulse(const char* message_format, const char* message_transport, int buffer_size, float threshold, int delay );
    ESP32Impulse(const char* message_format, const char* message_transport, int buffer_size, float threshold, int delay, std::vector<bool> mask);
    ESP32Impulse(const char* message_format, const char* message_transport, int buffer_size, float threshold, int delay, std::vector<bool> mask, int threshold_coord);


  ~ESP32Impulse();


  // Initialize the processor
  void Setup();

  // void CopyDataToMessage();

  void ProcessData(long t,
    float lat,float lon,
    float a, float b,float c,
    float d, float e,float f,
    float g, float h, float i);


  std::vector<bool> fOnOffs;

  // The data buffer
  std::vector< std::vector<float> > fMeasurements;

  int fThresholdCoord;
  int fBufferSize;
  float fBufferThreshold;
  int fBufferDelayCount;
  int fBufferDelay;
  bool fBufferDelayOn;

  bool fNoNames;

  int fDataCount;

  };


#endif /* ESP32Impulse_hpp */
