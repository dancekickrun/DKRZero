//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32FTPConnection.h"

ESP32FTPConnection::ESP32FTPConnection(const char* ssid, const char* password, int attempts) :
fWifiNetworkName(ssid), fWifiNetworkPassword(password), fAttempts(attempts)
{
  // From the base class
  ESP32Connection::fConnected = false;

  // Set the type
  ESP32Connection::ESP32ConnectionType = ConnectionType::FTP;

};

ESP32FTPConnection::~ESP32FTPConnection(){}

// Connect to the wifi network
void ESP32FTPConnection::Initialize()
{

  // check if connected then don't reinitialize
  if(WiFi.status() == WL_CONNECTED) return;

  // Print a message
  Serial.print("INFO: Wifi connecting to ");
  Serial.println(fWifiNetworkName);

  // Start the wifi connection
  WiFi.begin(fWifiNetworkName, fWifiNetworkPassword);

};

void ESP32FTPConnection::Start()
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

  // Start the FTP server
  StartFTPServer();

};


void ESP32FTPConnection::GetTime()
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


void ESP32FTPConnection::Stop()
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

  // Stop the FTP server
  StopFTPServer();

};



/**
 * FTP Server Related Functions
 */

void ESP32FTPConnection::StartFTPServer()
{
  fFTP = new FtpServer();
  fFTP->begin("esp32","esp32"); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
};

// In order to communicate with the FTP server put this in a loop...
void ESP32FTPConnection::CheckFTPServer()
{
  fFTP->handleFTP(); //make sure in loop you call handleFTP()!!
};

void ESP32FTPConnection::StopFTPServer()
{

};







void ESP32FTPConnection::Communicate(String message)
{
  Serial.println(message);
};
