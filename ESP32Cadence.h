// Copyright (c) 2018 by Chris Steer.
// All Rights Reserved.
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ESP32Cadence_hpp
#define ESP32Cadence_hpp

#include <vector>
#include <stdio.h>
#include "ESP32Processor.h"
#include "arduinoFFT.h"

class ESP32Cadence : public ESP32Processor {
public:
    ESP32Cadence(JsonObject& json_connection);
    ~ESP32Cadence();

    // Initialize the processor
    void Setup(JsonObject& json_connection);

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
