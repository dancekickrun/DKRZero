//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32Cadence_hpp
#define ESP32Cadence_hpp

#include <vector>
#include <stdio.h>
#include "ESP32Processor.h"
#include "arduinoFFT.h"

class ESP32Cadence : public ESP32Processor {
public:
    ESP32Cadence(const char* message_format,const char* message_transport);
    ~ESP32Cadence();

    // Initialize the processor
    void Setup();

    //
    void Append(float data);

    //
    void ProcessData(long, float data);
    void ProcessData(long t,float la, float lo,
      float ax,float ay,float az,
      float gx,float gy,float gz,
      float mx,float my,float mz);

    void Reset();

    float RMSMagnitude(float );
    //
    void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);

protected:

  // For the FFT
  double vReal[64];
  double vImag[64];

  //
  std::vector<float> fData;

  int fStartupCount;
  int fVerboseLevel;

  arduinoFFT fFFT;
  float fFFT_exponent;
  int fNPoints;
  float samplingFrequency;

  float fPeakFrequency;
  float fFFTIntegral;

private:

};


#endif /* ESP32Cadence_hpp */
