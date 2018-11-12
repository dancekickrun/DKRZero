//
//  ESP32Impulse.cpp
//  trigger
//
//  Created by Chris Steer on 13/08/2018.
//
//

#include "ESP32Impulse.h"

ESP32Impulse::ESP32Impulse(JsonObject& json_process)
{
  fBufferSize = 100;
  fBufferThreshold = 2.0;
  fBufferDelay = 50;

  for(int i=0;i<13;i++) fMeasurements.push_back(std::vector<float>(fBufferSize,0.0));

  // fOnOffs(mask),
  // fDataCount(0),
  // fThresholdCoord(threshold_coord)

};

// ESP32Impulse::ESP32Impulse(const char* message_format, const char* message_transport, int buffer_size, float threshold, int delay) : ESP32Processor(message_format, message_transport),
// fBufferSize(buffer_size),
// fBufferThreshold(threshold),
// fBufferDelay(delay),
// fMeasurements(9,std::vector<float>(buffer_size,0.0)),
// fOnOffs(std::vector<bool>(buffer_size,true)),
// fDataCount(0),
// fThresholdCoord(10)
// {};
//
// ESP32Impulse::ESP32Impulse(const char* message_format, const char* message_transport, int buffer_size, float threshold, int delay, std::vector<bool> mask) : ESP32Processor(message_format, message_transport),
// fBufferSize(buffer_size),
// fBufferThreshold(threshold),
// fBufferDelay(delay),
// fMeasurements(9,std::vector<float>(buffer_size,0.0)),
// fOnOffs(mask),
// fDataCount(0),
// fThresholdCoord(10)
// {};
//
// ESP32Impulse::ESP32Impulse(const char* message_format, const char* message_transport, int buffer_size, float threshold, int delay, std::vector<bool> mask, int threshold_coord) : ESP32Processor(message_format, message_transport),
// fBufferSize(buffer_size),
// fBufferThreshold(threshold),
// fBufferDelay(delay),
// fMeasurements(13,std::vector<float>(buffer_size,0.0)),
// fOnOffs(mask),
// fDataCount(0),
// fThresholdCoord(threshold_coord)
// {};

ESP32Impulse::~ESP32Impulse(){};

void ESP32Impulse::Setup(JsonObject& json_process)
{

  fNoNames=true;


  if(json_process.containsKey("buffer_size")) fBufferSize = atoi(json_process["buffer_size"]);
  if(json_process.containsKey("trigger_threshold")) fBufferThreshold = atoi(json_process["trigger_threshold"]);
  if(json_process.containsKey("trigger_delay")) fBufferDelay = atoi(json_process["trigger_delay"]);

  Serial.println("INFO: Setting up Impulse Processor:");
  Serial.print(" - Buffer Size ");
  Serial.println(fBufferSize);
  Serial.print(" - Buffer Delay ");
  Serial.println(fBufferDelay);
  Serial.print(" - Buffer Threshold ");
  Serial.println(fBufferThreshold);
  // Serial.print(" - Threshold Coord ");
  // Serial.println(fThresholdCoord);
  // Serial.print(" - Mask[0] ");
  // Serial.println(fOnOffs[0]);
  // Serial.print(" - Mask[1] ");
  // Serial.println(fOnOffs[1]);
  // Serial.print(" - Mask[2] ");
  // Serial.println(fOnOffs[2]);

  // Do not communicate until triggered
  fDataReady=false;
  fBufferDelayCount=0;
  fBufferDelayOn=false;

  for(int i=0; i<13; i++)
  {
    fMeasurements[i].clear();
    for(int j=0; j<fBufferSize; j++)
    {
      fMeasurements[i].push_back(0.0);
      // Serial.println("i ");
      // Serial.println(i);
      // Serial.println("j ");
      // Serial.println(j);
      // Serial.println("data ");
      // Serial.println(fMeasurements[i].at(j));
    }
  }

  fOnOffs[0]=true;fOnOffs[1]=false;fOnOffs[2]=false;
  fDelimiter = String(" ");

}

void ESP32Impulse::ProcessData(long t,float lat,float lon, float a, float b,float c, float d, float e,float f, float g, float h, float i)
{
  // Create a temporary vector with the tuple in it
  // std::vector<float> meas = {t,a,b,c,sqrt(a*a+b*b+c*c)};
  // BE CAREFUL MAX BUFFER SIZE IS 2040 elements = (2040/meas.size()) data points
  std::vector<float> meas = {(float) t,sqrt(a*a+b*b+c*c)};
  // Serial.println(a);
  fNelements=meas.size();

  // Copy the data straight into the message vector
  // - delete the first member if it is the same size as the buffer size*meas.size()
  if(ESP32Processor::MessageTransportData.size()>=(fBufferSize*((int) meas.size())))
    for(int i=0;i<meas.size();i++) ESP32Processor::MessageTransportData.erase(ESP32Processor::MessageTransportData.begin());

  // Append the data to the message data buffer
  for(int i=0;i<meas.size();i++) ESP32Processor::MessageTransportData.push_back((float) meas.at(i));

  // Serial.print(ESP32Processor::MessageTransportData.size());
  // Serial.print(" / ");
  // Serial.println((fBufferSize*((int) meas.size())));

  // // Delete and append the time
  // if(fMeasurements[0].size()==fBufferSize) fMeasurements[0].erase(fMeasurements[0].begin());
  // fMeasurements[0].push_back(t);
  //
  // // Serial.print(fMeasurements[0].size());
  // // Serial.print(" / ");
  // // Serial.println(fBufferSize);
  //
  // // Delete the first data of every measurement
  // for(int i=0; i<12; i++) if(fMeasurements[i+1].size()==fBufferSize) fMeasurements[i+1].erase(fMeasurements[i+1].begin());
  //
  // // Add the first data of every measurement if true
  // for(int i=0; i<12; i++) fMeasurements[i+1].push_back(meas.at(i));


  // In untriggered operation the unit should just append the data to the buffer (above)
  // and then maintain fDataReady as false until the trigger condition is met

  // When the trigger condition is met, we need to start the count towards the buffer delay
  // so set the flag fBufferDelayOn to be true
  if(meas.back()>fBufferThreshold) fBufferDelayOn = true;

  // If the count is less than the delay then keep the data as being not ready
  if(fBufferDelayCount<fBufferDelay)
  {
    ESP32Processor::fDataReady=false;

  } else {
    // Here the count is equal to or larger than the delay count so we want to write it out
    ESP32Processor::fDataReady = true;
    // Stop the delay count and reinitialize it to 0
    fBufferDelayOn = false;
    fBufferDelayCount=0;
  }

  // If the trigger condition has been met then keep counting the data into the buffer
  if(fBufferDelayOn) fBufferDelayCount++;

}


//
// void ESP32Impulse::CopyDataToMessage()
// {
//
//   ESP32Processor::MessageTransportNames.clear();
//   ESP32Processor::MessageTransportData.clear();
//
//   int k;
//   while(fMeasurements[0].size()!=0)
//   {
//
//
//     ESP32Processor::MessageTransportData.push_back(fMeasurements[0].at(0));
//     fMeasurements[0].erase(fMeasurements[0].begin());
//     Serial.println(fMeasurements[0].size());
//
//     for(int j=0; j<9; j++)
//     {
//       if(j<3) k=0; if((j>=3)&&(j<6)) k=1; if((j>=6)&&(k<9)) k=2;
//       if(fOnOffs[k]){
//         ESP32Processor::MessageTransportData.push_back(fMeasurements[j+1].at(0));
//         fMeasurements[j+1].erase(fMeasurements[j+1].begin());
//       }
//     }
//     if(fOnOffs[0]){
//       ESP32Processor::MessageTransportData.push_back(fMeasurements[10].at(0));
//       fMeasurements[10].erase(fMeasurements[10].begin());
//     }
//     if(fOnOffs[1]){
//       ESP32Processor::MessageTransportData.push_back(fMeasurements[11].at(0));
//       fMeasurements[11].erase(fMeasurements[11].begin());
//     }
//     if(fOnOffs[2]){
//       ESP32Processor::MessageTransportData.push_back(fMeasurements[12].at(0));
//       fMeasurements[12].erase(fMeasurements[12].begin());
//     }
//   }
//
// }


//   // Copy out the data to the message buffer
//   for(int i=0;i<fBufferSize;i++)
//   {
//
//     if(fNoNames){
//     ESP32Processor::MessageTransportNames.push_back("time");
//     if(fOnOffs[0])
//     {
//       ESP32Processor::MessageTransportNames.push_back("ax");
//       ESP32Processor::MessageTransportNames.push_back("ay");
//       ESP32Processor::MessageTransportNames.push_back("az");
//     }
//     if(fOnOffs[1])
//     {
//       ESP32Processor::MessageTransportNames.push_back("gx");
//       ESP32Processor::MessageTransportNames.push_back("gy");
//       ESP32Processor::MessageTransportNames.push_back("gz");
//     }
//     if(fOnOffs[2])
//     {
//       ESP32Processor::MessageTransportNames.push_back("mx");
//       ESP32Processor::MessageTransportNames.push_back("my");
//       ESP32Processor::MessageTransportNames.push_back("mz");
//     }
//
//     if(fOnOffs[0]) ESP32Processor::MessageTransportNames.push_back("mag_a");
//     if(fOnOffs[1]) ESP32Processor::MessageTransportNames.push_back("mag_g");
//     if(fOnOffs[2]) ESP32Processor::MessageTransportNames.push_back("mag_m");
//     }
//     //Append newline to the last string
//     // String s = ESP32Processor::MessageTransportNames.back() + String("\n");
//     // ESP32Processor::MessageTransportNames[ESP32Processor::MessageTransportNames.size()-1] = s;
//
//
//
//
//     // Now copy the data
//     int k;
//     for(int j=0; j<9; j++)
//     {
//       if(j<3) k=0; if((j>=3)&&(j<6)) k=1; if((j>=6)&&(k<9)) k=2;
//       if(fOnOffs[k])
//       {
//         ESP32Processor::MessageTransportData.push_back(fMeasurements[j+1].at(i));
//       }
//     }
//     if(fOnOffs[0]) ESP32Processor::MessageTransportData.push_back(fMeasurements[10].at(i));
//     if(fOnOffs[1]) ESP32Processor::MessageTransportData.push_back(fMeasurements[11].at(i));
//     if(fOnOffs[2]) ESP32Processor::MessageTransportData.push_back(fMeasurements[12].at(i));
//
//
//   }// end loop over data
//
// }// End of CopyDataToMessage

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// void ESP32Impulse::ProcessData(long t,
//   float lat,float lon,
//   float a, float b,float c,
//   float d, float e,float f,
//   float g, float h, float i)
//   {
//     // Serial.println("void ESP32Impulse::ProcessData");
//
//     // process->ProcessData(fCurrentTime,lat,lon,ax,ay,az,gx,gy,gz,mx,my,mz);
//
//     const char* amag_name = "acceleration_mag";
//     const char* ax_name = "acceleration_x";
//     const char* ay_name = "acceleration_y";
//     const char* az_name = "acceleration_z";
//
//     float data;
//     if (strcmp(fDataName, ax_name) == 0)
//     {
//       data = a;
//     }
//     else if (strcmp(fDataName, ay_name) == 0)
//     {
//       data = b;
//     }
//     else if (strcmp(fDataName, az_name) == 0)
//     {
//       data = c;
//     }
//     else// DEFAULT CASE IS ACCEL MAGNITUDE
//     {
//       //if (strcmp(fDataName, amag_name) == 0)
//       data = sqrt(a*a + b*b + c*c);
//     }
//
//     // Serial.print("fDataName ");Serial.println(fDataName);
//     // Serial.print("Cmp ");Serial.println(amag_name);
//
//     // Serial.print("Data : "); Serial.println(data);
//
//     // AppendData(t,data);
//     AppendMessage(t,data);
//     fDataCount++;
//
//     // When the trigger condition is met, we need to start the count towards the buffer delay
//     if(data>fBufferThreshold) fBufferDelayOn = true;
//
//     if(fBufferDelayCount<fBufferDelay) ESP32Processor::fDataReady=false;
//
//     if(fBufferDelayCount>=fBufferDelay){
//       ESP32Processor::fDataReady = true;
//       fBufferDelayOn = false;
//       fBufferDelayCount=0;
//     }
//
//     if(fBufferDelayOn) fBufferDelayCount++;
//
//     // Serial.print("Delay Count: "); Serial.println(fBufferDelayCount);
//     // Serial.print("Delay On: "); Serial.println(fBufferDelayOn);
//     // Serial.print("fDataCount : "); Serial.println(fDataCount);
//
//   };
//
//   bool ESP32Impulse::Trigger()
//   {
//
//
//     if(fDataBuffer.back()>fBufferThreshold) return true;
//
//     return false;
//
//   };
