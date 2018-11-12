//
//  ESP32Cadence.hpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#ifndef ESP32Connection_hpp
#define ESP32Connection_hpp

#include "Arduino.h"

// Base class for all data connections
class ESP32Connection {

public:

    ESP32Connection();
    ~ESP32Connection();

    // Initialize this manager
    virtual void Initialize() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

    virtual void Communicate(String)=0;

    enum class ConnectionType{Wifi, Bluetooth, BLE, FTP, SD, MQTT};
    ESP32Connection::ConnectionType GetType(){return ESP32ConnectionType;};
    bool IsType(ESP32Connection::ConnectionType testType){return (testType==GetType());};


protected:


    ConnectionType ESP32ConnectionType;

    bool fConnected;

private:

};


#endif /* ESP32Cadence_hpp */
