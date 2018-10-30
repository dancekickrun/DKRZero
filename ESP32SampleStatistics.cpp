//
//  ESP32SampleStatistics.cpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32SampleStatistics.h"

ESP32SampleStatistics::ESP32SampleStatistics(const char* message_format, const char* message_transport, int npoints, std::vector<bool> onoff):
ESP32Processor(message_format, message_transport),
fNPoints(npoints),
measurements(10,std::vector<float>(npoints,0.0)),
onoffs(onoff)
{
ESP32Processor::fDataReady = true;
}

ESP32SampleStatistics::~ESP32SampleStatistics()
{

}

void ESP32SampleStatistics::Setup()
{
  Serial.println("INFO: Setting up ESP32SampleStatistics ");
  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back("time");
  if(onoffs[0]) ESP32Processor::MessageTransportNames.push_back("mean_ax");
  if(onoffs[1]) ESP32Processor::MessageTransportNames.push_back("mean_ay");
  if(onoffs[2]) ESP32Processor::MessageTransportNames.push_back("mean_az");
  if(onoffs[3]) ESP32Processor::MessageTransportNames.push_back("mean_gx");
  if(onoffs[4]) ESP32Processor::MessageTransportNames.push_back("mean_gy");
  if(onoffs[5]) ESP32Processor::MessageTransportNames.push_back("mean_gz");
  if(onoffs[6]) ESP32Processor::MessageTransportNames.push_back("mean_mx");
  if(onoffs[7]) ESP32Processor::MessageTransportNames.push_back("mean_my");
  if(onoffs[8]) ESP32Processor::MessageTransportNames.push_back("mean_mz");

  if(onoffs[0]) ESP32Processor::MessageTransportNames.push_back("std_ax");
  if(onoffs[1]) ESP32Processor::MessageTransportNames.push_back("std_ay");
  if(onoffs[2]) ESP32Processor::MessageTransportNames.push_back("std_az");
  if(onoffs[3]) ESP32Processor::MessageTransportNames.push_back("std_gx");
  if(onoffs[4]) ESP32Processor::MessageTransportNames.push_back("std_gy");
  if(onoffs[5]) ESP32Processor::MessageTransportNames.push_back("std_gz");
  if(onoffs[6]) ESP32Processor::MessageTransportNames.push_back("std_mx");
  if(onoffs[7]) ESP32Processor::MessageTransportNames.push_back("std_my");
  if(onoffs[8]) ESP32Processor::MessageTransportNames.push_back("std_mz");


};



void ESP32SampleStatistics::ProcessData(long t,float la, float lo,
  float ax,float ay,float az,
  float gx,float gy,float gz,
  float mx,float my,float mz)
  {

    ESP32Processor::MessageTransportData.clear();
    ESP32Processor::MessageTransportData.push_back(t);

    // Delete the first sample from each type of measurement
    for(int i=0; i<9; i++) measurements[i].erase(measurements[i].begin());

    // Serial.println(measurements[0].size());

    // Append to the end for each type of measurement
    measurements[0].push_back(ax);measurements[1].push_back(ay);measurements[2].push_back(az);
    measurements[3].push_back(gx);measurements[4].push_back(gy);measurements[5].push_back(gz);
    measurements[6].push_back(mx);measurements[7].push_back(my);measurements[8].push_back(mz);

    // Update the output data vector
    for(int i=0; i<9; i++)
    {

        float tmp = 0;
        float tmp2 = 0;
        for(int j=0; j<fNPoints; j++)
        {
          tmp+= measurements[i][j];
          tmp2+= measurements[i][j]*measurements[i][j];
        }

        // Save the mean
        ESP32Processor::MessageTransportData.push_back(tmp/((float) fNPoints));
        // Save  the std dev.
        float var = (tmp2/((float) fNPoints)) - tmp*tmp;
        ESP32Processor::MessageTransportData.push_back(sqrt(var));

    }

  };

void ESP32SampleStatistics::ProcessData(long CurrentTime, float data_point){};
