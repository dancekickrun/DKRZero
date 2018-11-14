//
//  ESP32Processor.cpp
//  trigger
//
//  Created by Chris Steer on 11/08/2018.
//
//

#include "ESP32Processor.h"

ESP32Processor::ESP32Processor()
{
  // the names to compare with
  const char* format_space_name = "space";
  const char* format_spacenn_name = "space_no_names";
  const char* format_cols_name = "columns";
  const char* format_json_name = "json";

  fDataReady=true;
  fDelimiter=String(" ");
  fNelements=-1;

  const char* message_format = "space_no_names";
  const char* message_transport = "serial";

  // Return the Class of the processor if requested in the JSON
  if (strcmp(message_format, format_space_name) == 0) fMessageFormat = MessengerFormat::Space;
  if (strcmp(message_format, format_spacenn_name) == 0) fMessageFormat = MessengerFormat::Space_NoNames;
  if (strcmp(message_format, format_json_name) == 0) fMessageFormat = MessengerFormat::JSON;
  if (strcmp(message_format, format_cols_name) == 0) fMessageFormat = MessengerFormat::Columns;

  // Transport command names to compare with
  const char* transport_serial = "serial";
  const char* transport_mqtt = "mqtt";
  const char* transport_sdcard = "sdcard";

  // Return the Class of the processor if requested in the JSON
  if (strcmp(message_transport, transport_serial) == 0) fMessageTransport = MessengerTransport::Serial;
  if (strcmp(message_transport, transport_mqtt) == 0) fMessageTransport = MessengerTransport::MQTT;
  if (strcmp(message_transport, transport_sdcard) == 0) fMessageTransport = MessengerTransport::SDCard;

  // Serial.print("Format: ");
  // Serial.println((int) fMessageFormat);
  // Serial.print("Transport: ");
  // Serial.println((int) fMessageTransport);

}


    // void ESP32Processor::Communicate(ESP32Configuration* instance)
    // {
    //   // fDataReady must be true
    //   if(!fDataReady) return;
    //
    //   // First get the message
    //   String data_message = Message();
    //
    //   // Then second communicate
    //   switch (fMessageTransport)
    //   {
    //
    //     case MessengerTransport::Serial:
    //       Serial.println(data_message);
    //       break;
    //
    //     // case MessengerTransport::SDCard:
    //     //   instance->GetDataFile()->println(data_message);
    //     // break;
    //
    //     // case MessengerTransport::MQTT:
    //     //   // Serial.println("MessengerTransport::MQTT");
    //     //   char attributes[200];
    //     //   data_message.toCharArray( attributes, 200);
    //     //   // Serial.println(data_message);
    //     //   // Serial.println(attributes);
    //     //   (instance->GetPubSubClient())->publish( "v1/devices/me/telemetry", attributes );
    //     //   // Serial.println("ERROR: MQTT transport is not implemented yet");
    //     // break;
    //
    //   }
    //
    // };// End of Communicate()


ESP32Processor::~ESP32Processor() {}


String ESP32Processor::HeaderMessage()
{
  if(fNelements<0) fNelements = 1e9;

  String s;
  s+= String("# ");
  s+= String("Time(ms)");
  s+= fDelimiter;
  for(int i=0;i<MessageTransportNames.size(); i++)
  {
    s+= String(MessageTransportNames.at(i));
    s+= fDelimiter;
  }

  return s;
}


// String fDelimiter;
// int fNelements;
String ESP32Processor::Message()
{

  if(fNelements<0) fNelements = 1e9;

  String s;
  s = String(MessageTransportData[0],0);
  s += String(" ");

  int i=1;
  while(i < (int) MessageTransportData.size() )
  {
    // This is for the Impulse processor to format each line correctly.
    if(((i+1)%fNelements)==0)
    {
      s += String(MessageTransportData[i],6);
      s += String("\n");
      i++;
      if(i < (int) MessageTransportData.size() )
      {
        s += String(MessageTransportData[i],0);
        s += String(" ");
        i++;
      }
    } else {
      s += String(MessageTransportData[i],6);
      s += String(" ");
      i++;
    }

  }
  //
  //
  //
  //
  //
  // char add_data[300];
  //
  // snprintf(tmp_message, sizeof(tmp_message), "%12.0f ", (float) MessageTransportData[0]);
  //
  // // Serial.println((float) MessageTransportData[0]);
  // int i=1;
  //
  // // Serial.println((int) MessageTransportData.size());
  // while(i < (int) MessageTransportData.size() )
  // {
  //   // This is for the Impulse processor to format each line correctly.
  //   if(((i+1)%fNelements)==0)
  //   {
  //     snprintf(add_data, sizeof(add_data), "%6.4f \n", (float) MessageTransportData[i]);
  //     strcat(tmp_message,add_data);
  //     i++;
  //     snprintf(add_data, sizeof(add_data), "%12.0f ", (float) MessageTransportData[i]);
  //     strcat(tmp_message,add_data);
  //     i++;
  //   }else{
  //     snprintf(add_data, sizeof(add_data), "%6.4f ", (float) MessageTransportData[i]);
  //     strcat(tmp_message,add_data);
  //     i++;
  //   }
  //
  //   // Serial.print(i);
  //   // Serial.print(" ");
  //   // Serial.println(MessageTransportData[i]);
  // }

  // snprintf(add_data, sizeof(add_data), "\n");
  // strcat(tmp_message,add_data);

  // Serial.println(String(tmp_message));

  return String(s);

}


  // String s("");
  // Serial.print("Time: ");
  // Serial.println(fCurrentTime);
  // s+= fCurrentTime;
  // s+= fDelimiter;
    // Serial.println(i);
    // Serial.println(MessageTransportData.at(i));
    // if(i==0)
    // {
    //   s+= String((int) MessageTransportData.at(i));
    // }
    // else
    // {
    //   s+= String(MessageTransportData.at(i),6);
    // // }
    // s+= fDelimiter;
    // if(((i+1)%fNelements)==0) s+= String("\n");
  // }
  // return s;
// }

// String ESP32Processor::Message() {
//
//   int nelements = (int) MessageTransportNames.size();
//
//   String s;
//
//   switch (fMessageFormat)
//   {
//
//     case MessengerFormat::Space:
//       // Serial.println("Space");
//
//       s+=fCurrentTime;
//       s+= " ";
//       // Serial.println(s);
//       for(int i=0;i<nelements; i++)
//       {
//         s+= MessageTransportNames.at(i);
//         s+= " ";
//         s+= String(MessageTransportData.at(i),6);
//         s+= " ";
//       }
//       // Serial.println(s);
//       break;
//
//       // Written for impulse processor
//       case MessengerFormat::Columns:
//       // Serial.println("Space");
//
//       // s+=fCurrentTime;
//       // s+= " ";
//       // Serial.println(s);
//       for(int i=0;i<nelements-1; i+=2)
//       {
//         // s+= MessageTransportNames.at(i);
//         s+= " ";
//         s+= String(MessageTransportData.at(i),6);
//         s+= " ";
//         s+= String(MessageTransportData.at(i+1),6);
//         s+= "\n";
//       }
//       Serial.println(s);
//       break;
//
//     case MessengerFormat::Space_NoNames:
//
//     s+=fCurrentTime;
//     s+= " ";
//     // Serial.println(s);
//     for(int i=0;i<nelements; i++)
//     {
//       s+= String(MessageTransportData.at(i),6);
//       s+= " ";
//     }
//     // Serial.println(s);
//     break;
//
//
//     case MessengerFormat::JSON:
//       s = JSONMessage();
//       break;
//
//   }
//   // Serial.println(s);
//   return s;
// };
