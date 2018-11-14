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

#include "ESP32SampleFrequency.h"

ESP32SampleFrequency::ESP32SampleFrequency(JsonObject& json_process)
{
  fNPoints=20;

  Setup(json_process);
}

ESP32SampleFrequency::~ESP32SampleFrequency()
{

}

void ESP32SampleFrequency::Setup(JsonObject& json_process)
{
  Serial.println("INFO: Setting up ESP32SampleFrequency ");
  ESP32Processor::MessageTransportNames.clear();
  ESP32Processor::MessageTransportNames.push_back("sample_frequency");

  ESP32Processor::MessageTransportData.push_back(0.0);
  ESP32Processor::MessageTransportData.push_back(0.0);

  if(json_process.containsKey("num_points")) fNPoints = atoi(json_process["num_points"]);
  if(fNPoints<10) fNPoints=10;

  //for(int i=0;i<fNPoints;i++) tmp_data.push_back((long) 0);
  tmp_data.clear();

};



void ESP32SampleFrequency::ProcessData(long t,float la, float lo, float ax,float ay,float az, float gx,float gy,float gz,float mx,float my,float mz)
{

    ESP32Processor::MessageTransportData[0]=t;

    // remove the first element
    if(tmp_data.size()>=fNPoints) tmp_data.erase(tmp_data.begin());

    // add to the last element
    tmp_data.push_back(t);

    if(tmp_data.size()>=10)
    {
      std::vector<long> median_data;
      for(int i=0;i<(int) tmp_data.size()-1; i++) median_data.push_back(tmp_data[i+1]-tmp_data[i]);
      ESP32Processor::MessageTransportData[1] = 1000.0/Median(median_data);
    } else {
      ESP32Processor::MessageTransportData[1]= (float) 0.0;
    }

    // Calculate the average time difference
    // if(tmp_data.size()>=10)
    // {
    //   fDiffTime=0;
    //   for(int j=0;j<tmp_data.size()-1;j++) fDiffTime += ((float) tmp_data[j+1] - (float) tmp_data[j]);
    //   fDiffTime /= ((float) (tmp_data.size()-1));
    //   ESP32Processor::MessageTransportData[1]= ((float) 1000.0) / ((float) fDiffTime);
    // }
    // else
    // {
    //   ESP32Processor::MessageTransportData[1]= (float) 0.0;
    // }

    // Serial.println(ESP32Processor::MessageTransportData[1]);

};

void ESP32SampleFrequency::ProcessData(long CurrentTime, float data_point){};



float ESP32SampleFrequency::Median(std::vector<long> data) {
    int temp;

    int n = (int) data.size();

    // the following two loops sort the array x in ascending order
    for(int i=0; i<n-1; i++) {
        for(int j=i+1; j<n; j++) {
            if(data[j] < data[i]) {
                // swap elements
                std::swap(data[i],data[j]);
                // temp = x[i];
                // x[i] = x[j];
                // x[j] = temp;
            }
        }
    }

    
    if(n%2==0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return ((float) (data[n/2] + data[n/2 - 1]) / 2.0);
    } else {
        // else return the element in the middle
        return ((float) data[n/2]);
    }
};
