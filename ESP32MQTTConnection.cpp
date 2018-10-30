//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32MQTTConnection.h"

ESP32MQTTConnection::ESP32MQTTConnection( const char* wifi_ssid, const char* wifi_password, int wifi_attempts,
  const char* mqtt_server, const int mqtt_port,const char* mqtt_thing_name,const char* mqtt_thing_access_token) :
  fWifiNetworkName(wifi_ssid), fWifiNetworkPassword(wifi_password), fAttempts(wifi_attempts),
  fMQTTServerIP(mqtt_server), fMQTTThingName(mqtt_thing_name), fMQTTAccessToken(mqtt_thing_access_token), fMQTTServerPort(mqtt_port),
  fPubSubClient(fPubSubWifiClient)
  {
    // From the base class
    ESP32Connection::fConnected = false;

    // Set the type
    ESP32Connection::ESP32ConnectionType = ConnectionType::MQTT;

  };

  ESP32MQTTConnection::~ESP32MQTTConnection(){}

  // Connect to the wifi network
  void ESP32MQTTConnection::Initialize()
  {

    // check if connected then don't reinitialize
    if(WiFi.status() == WL_CONNECTED) return;

    // Print a message
    Serial.print("INFO: Wifi connecting to ");
    Serial.println(fWifiNetworkName);

    // Start the wifi connection
    WiFi.begin(fWifiNetworkName, fWifiNetworkPassword);

  };




  void ESP32MQTTConnection::Start()
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

      Serial.print("Connecting to MQTT...");
      fPubSubClient.setServer(fMQTTServerIP,fMQTTServerPort);
      while (!fPubSubClient.connected()) {
        if ( fPubSubClient.connect(fMQTTThingName, fMQTTAccessToken, NULL) ) {
          Serial.println( "[DONE]" );
        } else {
          Serial.print( "[FAILED] [ rc = " );
          Serial.print( fPubSubClient.state() );
          Serial.println( " : retrying in 1 second]" );
          // Wait 1 second before retrying
          delay( 1000 );
        }
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


  void ESP32MQTTConnection::GetTime()
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


  void ESP32MQTTConnection::Stop()
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


  void ESP32MQTTConnection::Communicate( String message)
  {
    Serial.println(message);
  };
