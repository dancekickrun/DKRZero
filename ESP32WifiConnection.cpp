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


#include "ESP32WifiConnection.h"

ESP32WifiConnection::ESP32WifiConnection(const char* ssid, const char* password, int attempts) :
fWifiNetworkName(ssid), fWifiNetworkPassword(password), fAttempts(attempts)
{
  // From the base class
  ESP32Connection::fConnected = false;

  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::Wifi;

};

ESP32WifiConnection::~ESP32WifiConnection(){}

// Connect to the wifi network
void ESP32WifiConnection::Initialize()
{

  // check if connected then don't reinitialize
  if(WiFi.status() == WL_CONNECTED) return;

  // Print a message
  Serial.print("INFO: Wifi connecting to ");
  Serial.println(fWifiNetworkName);

  // Start the wifi connection
  WiFi.begin(fWifiNetworkName, fWifiNetworkPassword);

};




void ESP32WifiConnection::Start()
{

  // Attempt to connect a number of times (infinite if fAttempts<0)
  Serial.print("INFO: WiFi connecting...");
  for(int i=0; i<fAttempts;i++){

    delay(500);
    Serial.print(".");
    if(WiFi.status() == WL_CONNECTED)
    {
      ESP32Connection::fConnected = true;
      Serial.println("done");
      break;
    }

  }


  // Get the IP address and the network time
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("INFO: IP address: ");
    fWifiIPAddress = WiFi.localIP().toString();
    Serial.println(fWifiIPAddress);

    // Update the current time from the network
    GetTime();

  } else {
    Serial.println("INFO: Continuing with WiFi disconnected");
  }

  // Set up a webserver for the unit?
  //
  // Set up the FTP server?



};


void ESP32WifiConnection::GetTime()
{

  // Check if we're connected first
  if(WiFi.status() != WL_DISCONNECTED)
  {

    // Now get the date and time from a central server
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("\n INFO: Waiting to connect to time server...");
    while (!time(nullptr))
    {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("done");

    // Print out the current time
    time_t fNetworkTime = time(nullptr);
    Serial.print("INFO: The time is ");
    Serial.println(ctime(&fNetworkTime));

  }

}


void ESP32WifiConnection::Stop()
{

  // Stop the connection
  WiFi.disconnect();
  Serial.print("INFO: WiFi disconnecting...");
  while (WiFi.status() != WL_DISCONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("done");

  ESP32Connection::fConnected = false;


};


void ESP32WifiConnection::Communicate( String message)
{
  Serial.println(message);
};
