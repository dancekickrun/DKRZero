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


#include "ESP32uWave.h"

ESP32uWave::ESP32uWave(JsonObject& json_process)
{

  // The dictionary file name
  fDictionaryFile = "dictionary.txt";

  //
  ESP32Processor::fDataReady = false;

  // Setup the processor
  Setup(json_process);

}

ESP32uWave::~ESP32uWave() {}


void ESP32uWave::Setup(JsonObject& json_process){

  // Load the dictionary of templates here
  Serial.println(String("INFO: uWave opening dictionary file "));
  // if(json_process.containsKey("dictionary")) fDictionaryFile = json_process["dictionary"]);

  // Open the file
  File file = SD.open(fDictionaryFile);

  // Check if the file hasn't been found here
  if(!file)
  {
    while(1)
    {
      Serial.println(String("ERROR: Failed to open uWave dictionary file."));
      delay(3000);
    }
  }

  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t bufferSize = JSON_OBJECT_SIZE(28) + 780;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  // Parse the root object
  JsonObject& root = jsonBuffer.parseObject(file);

  // Throw error if fail
  if (!root.success())
  {
    while(1)
    {
      Serial.println(String("Failed to read uWave dictionary file."));
      delay(3000);
    }
  }

  if(root.containsKey("templates"))
  {

      // Get the template structure from the file
      JsonArray& json_templates = root["templates"];

      // Loop over each one to find the data of each template
      for (auto& uWave_template : json_templates)
      {
        // Get the name of the template
        const char* name = uWave_template["name"];

        Serial.print("INFO: Found template name ");
        Serial.println(String(name));

        fTemplateNames.push_back(String(name));

        // Get the data
        ReadAccelerometerData(uWave_template);
        ReadGyroscopeData(uWave_template);
        ReadMagnetometerData(uWave_template);

      }

  } else {

    while(1)
    {
      Serial.println(String("ERROR: uWave dictionary file contains no templates."));
      delay(3000);
    }

  }

};



void ESP32uWave::ReadAccelerometerData(JsonObject& uWave_template)
{

  if(uWave_template.containsKey("accelerometer"))
  {
    Serial.println("INFO: Found accelerometer template data ");

    int number_datapoints =  uWave_template["accelerometer"].size();
    Serial.print("INFO: - "); Serial.print(number_datapoints); Serial.println(" datapoints found");

    std::vector<int> data;
    for (int i = 0; i < number_datapoints; i++)
    {
      Serial.print("INFO: - "); Serial.print(i); Serial.print(" "); Serial.print((float) uWave_template["accelerometer"][i]); Serial.println("");
      data.push_back(uWave_template["accelerometer"][i]);
    }
    fAccelerationTemplates.push_back(data);
  }

};


void ESP32uWave::ReadGyroscopeData(JsonObject& uWave_template)
{

  if(uWave_template.containsKey("gyroscope"))
  {
    Serial.println("INFO: Found gyoscope template data ");

    int number_datapoints =  uWave_template["gyroscope"].size();
    Serial.print("INFO: - "); Serial.print(number_datapoints); Serial.println(" datapoints found");

    std::vector<int> data;
    for (int i = 0; i < number_datapoints; i++)
    {
      Serial.print("INFO: - "); Serial.print(i); Serial.print(" "); Serial.print((float) uWave_template["gyroscope"][i]); Serial.println("");
      data.push_back(uWave_template["gyroscope"][i]);
    }
    fGyroscopeTemplates.push_back(data);
  }

};


void ESP32uWave::ReadMagnetometerData(JsonObject& uWave_template)
{

  if(uWave_template.containsKey("magnetometer"))
  {
    Serial.println("INFO: Found magnetometer template data ");

    int number_datapoints =  uWave_template["magnetometer"].size();
    Serial.print("INFO: - "); Serial.print(number_datapoints); Serial.println(" datapoints found");

    std::vector<int> data;
    for (int i = 0; i < number_datapoints; i++)
    {
      Serial.print("INFO: - "); Serial.print(i); Serial.print(" "); Serial.print((float) uWave_template["magnetometer"][i]); Serial.println("");
      data.push_back(uWave_template["magnetometer"][i]);
    }
    fMagnetometerTemplates.push_back(data);
  }

};


void ESP32uWave::ProcessData(long t,float la, float lo,
  float ax,float ay,float az,
  float gx,float gy,float gz,
  float mx,float my,float mz)
{

};



void ESP32uWave::ProcessData(long CurrentTime, float data_point){};
