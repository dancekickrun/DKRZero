//
//  ESP32Cadence.cpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32Cadence.h"
#include "arduinoFFT.h"

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

ESP32Cadence::ESP32Cadence(const char* message_format,const char* message_transport) :
ESP32Processor(message_format, message_transport), fNPoints(64), fVerboseLevel(0), fStartupCount(0)
{}

ESP32Cadence::~ESP32Cadence() {}

void ESP32Cadence::Setup(){

  Reset();
  fFFT = arduinoFFT();
  fFFT_exponent = fFFT.Exponent(fNPoints);
  samplingFrequency = 10;

  ESP32Processor::MessageTransportNames.push_back(String("peak_frequency"));
  ESP32Processor::MessageTransportNames.push_back(String("fft_integral"));
  // ESP32Processor::MessageTransport = ESP32Processor::MessengerFormat::Space;

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

};


void ESP32Cadence::Reset()
{
  // Serial.println("ESP32Cadence::Reset");

  for(int i=0;i<fNPoints;i++){
    fData.push_back(0.0);
    vReal[i] = 0.0;
    vImag[i] = 0.0;
  }
}

void ESP32Cadence::PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
	break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / fNPoints);
	break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}

//
void ESP32Cadence::Append(float data_point)
{
  // Serial.println("ESP32Cadence::Append");

  fStartupCount++;

  if((int) fData.size()>= (int) fNPoints){
    fData.erase(fData.begin());
    fData.push_back(data_point);
  } else {
    fData.push_back(data_point);
  }
  // Serial.print("DATA: ");
  // Serial.println( (int) fData.size() );

  // Init the real data
  for(int i=0;i<fNPoints;i++) vReal[i] = fData.at(i);

}



void ESP32Cadence::ProcessData(long t,float la, float lo,
  float ax,float ay,float az,
  float gx,float gy,float gz,
  float mx,float my,float mz)
  {
    // Serial.println("ESP32Cadence::ProcessData()");
    ProcessData(t, sqrt(ax*ax+ay*ay+az*az));
  };

//
void ESP32Cadence::ProcessData(long CurrentTime, float data_point){

  // Reset the imaginary part
  for(int i=0;i<fNPoints;i++) vImag[i] = 0.0;

  // Save the current time
  ESP32Processor::fCurrentTime = CurrentTime;

  if(fVerboseLevel>0) Serial.println("ESP32Cadence::ProcessData");

  Append(data_point);

  if(fStartupCount<fNPoints) return;

  if(fVerboseLevel>0) Serial.println("ESP32Cadence::FFT.Windowing");
  fFFT.Windowing(vReal, fNPoints, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
  if(fVerboseLevel>0) Serial.println("ESP32Cadence::FFT.Compute");
  fFFT.Compute(vReal, vImag, fNPoints, fFFT_exponent, FFT_FORWARD); /* Compute FFT */
  if(fVerboseLevel>0) Serial.println("ESP32Cadence::FFT.ComplexToMagnitude");
  fFFT.ComplexToMagnitude(vReal, vImag, fNPoints); /* Compute magnitudes */

  // PrintVector(vReal, (fNPoints >> 1), SCL_FREQUENCY);
  fPeakFrequency = fFFT.MajorPeak(vReal, fNPoints, samplingFrequency);
  fFFTIntegral = RMSMagnitude((float) 0.0);


  ESP32Processor::MessageTransportData.at(0) = fPeakFrequency;
  ESP32Processor::MessageTransportData.at(1) = fFFTIntegral;

};


float ESP32Cadence::RMSMagnitude(float freq_thresh)
{

  float sum=0;
  float freq;
  for(int i=0;i<fNPoints;i++)
  {
    freq = ((i * 1.0 * samplingFrequency) / fNPoints);
    if(freq>freq_thresh) sum+=(vReal[i]*vReal[i]+vImag[i]*vImag[i]);
  }

  return sqrt(sum);

};
