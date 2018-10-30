//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32BluetoothConnection.h"

ESP32BluetoothConnection::ESP32BluetoothConnection(): fUnitName(String("ESP32"))
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::Bluetooth;

};

ESP32BluetoothConnection::ESP32BluetoothConnection(String name): fUnitName(name)
{
  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::Bluetooth;

};


ESP32BluetoothConnection::~ESP32BluetoothConnection(){}

void ESP32BluetoothConnection::Initialize()
{
  Serial.println("INFO: Initializing bluetooth connection, connection name: ");
  SerialBT.begin(fUnitName); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

};

void ESP32BluetoothConnection::Start()
{
  Serial.println("INFO: Starting BLE connection");
};

void ESP32BluetoothConnection::Stop(){



};

void ESP32BluetoothConnection::Communicate(String message)
{
  SerialBT.println(message);
};
