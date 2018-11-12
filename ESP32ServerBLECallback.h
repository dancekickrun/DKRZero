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
