//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32BLECallback_hpp
#define ESP32BLECallback_hpp

#include <vector>
#include <stdio.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>



// BLE connections class
class ESP32BLECallback : public BLECharacteristicCallbacks
{

public:

  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      Serial.println("*********");
      Serial.print("New value: ");
      for (int i = 0; i < value.length(); i++)
        Serial.print(value[i]);

      Serial.println();
      Serial.println("*********");
    }
  }

private:


};


#endif
