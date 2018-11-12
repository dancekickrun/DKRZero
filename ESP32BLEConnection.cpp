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

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#include "ESP32BLEConnection.h"

ESP32BLEConnection::ESP32BLEConnection(JsonObject& json_connection): fUnitName(String("ESP32"))
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::BLE;

  Setup(json_connection);
};

ESP32BLEConnection::~ESP32BLEConnection(){}

void ESP32BLEConnection::Setup(JsonObject& json_connection)
{

  if(json_connection.containsKey("bluetooth_name")){
    const char* name = json_connection["bluetooth_name"];
    fUnitName = String(name);
  }

}




void ESP32BLEConnection::Initialize()
{
  Serial.println("INFO: Initializing bluetooth connection, connection name: ");
  // SerialBT.begin(fUnitName); //Bluetooth device name
  // Serial.println("The device started, now you can pair it with bluetooth!");

  pServer = NULL;
  fOldDeviceConnected = false;
  uint8_t txValue = 0;


  // BLEDevice::init("ESP32");
  // pServer = BLEDevice::createServer();
  // pService = pServer->createService(SERVICE_UUID);

  // Create the BLE Device
  BLEDevice::init("UART Service");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  fBLEServerCallback = new ESP32ServerBLECallback();
  fBLEServerCallback->SetDeviceConnected(false);
  pServer->setCallbacks(fBLEServerCallback);

  // Create the BLE Service
  pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,
  										BLECharacteristic::PROPERTY_NOTIFY
  									);

  pTxCharacteristic->addDescriptor(new BLE2902());

  pRxCharacteristic = pService->createCharacteristic(
  											 CHARACTERISTIC_UUID_RX,
  											BLECharacteristic::PROPERTY_WRITE
  										);

  fBLECallback = new ESP32BLECallback();
  pRxCharacteristic->setCallbacks(fBLECallback);


  // Serial.begin(115200);
  //
  // // Create the BLE Device
  // BLEDevice::init("ESP32");
  //
  // // Create the BLE Server
  // pServer = BLEDevice::createServer();
  //
  // // Define the service
  // pService = pServer->createService(fServiceUUID);

  // Create a BLE Characteristic
  // pCharacteristic = pService->createCharacteristic(fCharacteristicUUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  // pCharacteristic->setCallbacks(new ESP32BLECallback());
  // pCharacteristic->setValue("Hello World");


};

void ESP32BLEConnection::Start()
{
  Serial.println("INFO: Starting BLE connection");
  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

  // // Start the service
  // pService->start();
  //
  // // Advertise for connection
  // pAdvertising = pServer->getAdvertising();
  // pAdvertising->start();
  // Serial.println("Waiting a client connection to notify...");
  //
};

void ESP32BLEConnection::Stop(){};

void ESP32BLEConnection::Communicate(String message)
{

  if(fBLEServerCallback->GetDeviceConnected())
  {
        Serial.println("Connected Print");
        // Convert the message to a byte array
        //         byte plain[message.length()];

        // Loop over the bytes in message
        byte* plain;
        for(int ib=0;ib<message.length();ib++)
        {
          message.getBytes(plain, 1);
          pTxCharacteristic->setValue(plain, 1);
        }
        pTxCharacteristic->notify();
	      delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

    // disconnecting
    if (!fBLEServerCallback->GetDeviceConnected() && fOldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        fOldDeviceConnected = fBLEServerCallback->GetDeviceConnected();
    }

    // connecting
    if (fBLEServerCallback->GetDeviceConnected() && !fOldDeviceConnected) {
		    // do stuff here on connecting
        fOldDeviceConnected = fBLEServerCallback->GetDeviceConnected();
    }

  // SerialBT.println(message);
};
