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

#include "ESP32SampleStatistics.h"

ESP32SampleStatistics::ESP32SampleStatistics(JsonObject& json_process)
{
  // Initialize all default values of variables
  ESP32Processor::fDataReady = true;
  fNPoints=20;
  for(int i=0;i<10;i++) measurements.push_back(std::vector<float>(fNPoints,0.0));


  Setup(json_process);

}

ESP32SampleStatistics::~ESP32SampleStatistics()
{

}

void ESP32SampleStatistics::Setup(JsonObject& json_process)
{
  Serial.println("INFO: Setting up ESP32SampleStatistics ");
  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back("time");
  ESP32Processor::MessageTransportNames.push_back("mean_ax");
  ESP32Processor::MessageTransportNames.push_back("mean_ay");
  ESP32Processor::MessageTransportNames.push_back("mean_az");
  ESP32Processor::MessageTransportNames.push_back("mean_gx");
  ESP32Processor::MessageTransportNames.push_back("mean_gy");
  ESP32Processor::MessageTransportNames.push_back("mean_gz");
  ESP32Processor::MessageTransportNames.push_back("mean_mx");
  ESP32Processor::MessageTransportNames.push_back("mean_my");
  ESP32Processor::MessageTransportNames.push_back("mean_mz");

  ESP32Processor::MessageTransportNames.push_back("std_ax");
  ESP32Processor::MessageTransportNames.push_back("std_ay");
  ESP32Processor::MessageTransportNames.push_back("std_az");
  ESP32Processor::MessageTransportNames.push_back("std_gx");
  ESP32Processor::MessageTransportNames.push_back("std_gy");
  ESP32Processor::MessageTransportNames.push_back("std_gz");
  ESP32Processor::MessageTransportNames.push_back("std_mx");
  ESP32Processor::MessageTransportNames.push_back("std_my");
  ESP32Processor::MessageTransportNames.push_back("std_mz");


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
