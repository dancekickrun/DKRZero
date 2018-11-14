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

#include "ConnectionFactory.h"

ESP32Connection* ConnectionFactory::ConstructConnection(JsonObject& json_sensor)
{

  // Get the name of the sensor
  const char* name = json_sensor["name"];
  Serial.print("INFO: Found connection ");  Serial.println(name);

  const char* blue_connection_name = "bluetooth";
  if (strcmp(name, blue_connection_name) == 0)
  {
      Serial.println("INFO: Connection bluetooth created");
      return new ESP32BLEConnection(json_sensor);
  }

  // const char* wifi_process_name = "wifi";
  // if (strcmp(name, wifi_process_name) == 0) return new ESP32WifiConnection(json_sensor);

  const char* sd_connection_name = "sdcard";
  if (strcmp(name, sd_connection_name) == 0)
  {
      Serial.println("INFO: Connection sdcard created");
      return new ESP32SDCardConnection(json_sensor);
  }

  const char* serial_connection_name = "serial";
  if (strcmp(name, serial_connection_name) == 0)
  {
      Serial.println("INFO: Connection serial created");
      return new ESP32SerialConnection(json_sensor);
  }

  Serial.print("INFO: Unknown connection name -> "); Serial.println(name);


}
