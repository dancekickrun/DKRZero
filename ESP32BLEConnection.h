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

#ifndef ESP32BLEConnection_hpp
#define ESP32BLEConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>
#include <ArduinoJson.h>

#include "BluetoothSerial.h"

#include "ESP32BLECallback.h"
#include "ESP32ServerBLECallback.h"

#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"



#include "ESP32Connection.h"

// BLE connections class
class ESP32BLEConnection : public ESP32Connection {

public:

    ESP32BLEConnection(JsonObject& json_sensor);
    ESP32BLEConnection(String);
    ~ESP32BLEConnection();

    void Setup(JsonObject& json_sensor);

    void Initialize();
    void Start();
    void Stop();
    void Communicate(String );

private:

  String fUnitName;

  // Common information to all connections
  const char* fCharacteristicUUID;
  const char* fServiceUUID;
  const char* fMode;

  // The service pointer
  BLEService *pService;

  // And the server
  BLEServer *pServer;

  // The advertising of the BLE connection
  BLEAdvertising *pAdvertising;

  // Characteristic of the service
  BLECharacteristic *pTxCharacteristic;
  BLECharacteristic *pRxCharacteristic;

  // Callbacks
  ESP32BLECallback* fBLECallback;
  ESP32ServerBLECallback* fBLEServerCallback;

  // BluetoothSerial SerialBT;
  bool fDeviceConnected;
  bool fOldDeviceConnected;

};


#endif
