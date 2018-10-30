//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32BLEConnection_hpp
#define ESP32BLEConnection_hpp

#include "Arduino.h"
#include <vector>
#include <stdio.h>

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

    ESP32BLEConnection();
    ESP32BLEConnection(String);
    ~ESP32BLEConnection();

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
