//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32ServerBLECallback_hpp
#define ESP32ServerBLECallback_hpp

#include <vector>
#include <stdio.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>



// BLE connections class
class ESP32ServerBLECallback : public BLEServerCallbacks
{

public:

  void onConnect(BLEServer* pServer) {
       fDeviceConnected = true;
     };

     void onDisconnect(BLEServer* pServer) {
       fDeviceConnected = true;
     }

     bool GetDeviceConnected(){return fDeviceConnected;};
     void SetDeviceConnected(bool dc){fDeviceConnected=dc;};

private:

  bool fDeviceConnected;

};


#endif
