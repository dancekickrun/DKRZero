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

#ifndef ESP32Sensor_hpp
#define ESP32Sensor_hpp

#include <vector>
#include <stdio.h>
#include <ArduinoJson.h>
#include <SparkFunLSM9DS1.h>

// Simple class to represent a sensor
class ESP32Sensor {

public:

    ESP32Sensor(){};
    ~ESP32Sensor(){};

    virtual void Setup(JsonObject& root) = 0;
    virtual void ConfigureIMU(LSM9DS1* imu) = 0;

    bool GetEnable(){return enable;};
    bool GetEnableX(){return enable_x;};
    bool GetEnableY(){return enable_y;};
    bool GetEnableZ(){return enable_z;};
    int GetScale(){return scale;};
    int GetSampleRate(){return sample_rate;};

    enum class SensorType{Accelerometer, Gyroscope, Magnetometer, GPS};

    ESP32Sensor::SensorType GetType(){return fSensorType;};
    bool IsType(ESP32Sensor::SensorType testType){return (testType==GetType());};





protected:

    SensorType fSensorType;

    bool enable;
    bool enable_x;
    bool enable_y;
    bool enable_z;

    int scale; //
    int sample_rate;

};


#endif /* ESP32Sensor_hpp */
