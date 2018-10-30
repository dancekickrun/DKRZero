//
//  ESP32RawData.hpp
//  trigger
//
//  Created by Chris Steer on 14/08/2018.
//
//

#ifndef ESP32RawData_hpp
#define ESP32RawData_hpp

#include <stdio.h>
#include "ESP32Processor.h"

class ESP32RawData : public ESP32Processor {
private:

public:

    ESP32RawData(const char* message_format,const char* message_transport);
    ~ESP32RawData();

    // Initialize the processor
    void Setup();

    //
    void ProcessData(long, float data);
    void ProcessData(long, float , float, float, float, float, float, float, float, float, float, float);

    float ax,ay,az,gx,gy,gz,mx,my,mz;
    float fLatitude,fLongitude;
    long fCurrentTime;



};


#endif /* ESP32RawData_hpp */
