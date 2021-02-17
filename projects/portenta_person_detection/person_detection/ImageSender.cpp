/*
Copyright 2021 Chariot Solutions Inc. (https://chariotsolutions.com)
and ARM (https://arm.com) All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "ImageSender.h"
#include "config.h"

#include <WiFi.h>

#define IS_CONNECT_TIMEOUT 3000
#define IS_SEND_TIMEOUT 5000
// #define IS_LOG_SERVER_RESPONSE

IPAddress server;

int status = WL_IDLE_STATUS;
WiFiClient client;

TfLiteStatus ImageSender::connect_to_wifi(tflite::ErrorReporter* error_reporter)
{

    if ( !server.fromString(SERVER_IP_ADDRESS) ) {
       TF_LITE_REPORT_ERROR(error_reporter, "Bad Server Address in SECRET_SERVER_IP_ADDRESS %s", SERVER_IP_ADDRESS);
        return kTfLiteError;
    }

    if (WiFi.status() == WL_NO_SHIELD) {
        TF_LITE_REPORT_ERROR(error_reporter, "Communication with WiFi module failed!");
        return kTfLiteError;
    }

   // TODO - fail after a certain amount of time?
   while (status != WL_CONNECTED) {
    TF_LITE_REPORT_ERROR(error_reporter, "Attempting to connect to SSID: %s", WIFI_SSID);
    
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);

    // wait for connection to AP:
    delay(IS_CONNECT_TIMEOUT);
  }

  print_wifi_status(error_reporter);
  TF_LITE_REPORT_ERROR(error_reporter, "Wifi Network Connected");
  
  return kTfLiteOk;
}

TfLiteStatus ImageSender::send_image(tflite::ErrorReporter* error_reporter, const uint8_t *image, int width, int height, int person_score, int no_person_score)
{
  int length = width * height;
  
  TF_LITE_REPORT_ERROR(error_reporter, "\nStarting connection to server...");
 
  if (client.connect(server, IS_SEND_TIMEOUT)) {
    TF_LITE_REPORT_ERROR(error_reporter,"connected to server");
    
    // Make a HTTP request:
    char buff[255] = {0};
    sprintf(buff, "POST /tf/%d/%d/%d/%d HTTP/1.1", width, height, person_score, no_person_score); 
    client.println(buff);
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(length);
    client.println();
    client.write(image, length);

    // Read the server's response - must read even if not logging
    #ifdef IS_LOG_SERVER_RESPONSE
        char response_buff[1024] = {0};
        char ch[2] = {0};

        while (client.available()) {
          ch[0] = client.read();
          if (strlen(response_buff) < sizeof(response_buff) - 2) {
              strncat(response_buff, ch, 1);
          }
        }
        TF_LITE_REPORT_ERROR(error_reporter, response_buff);
    #else
        while (client.available()) {
          client.read();
        }
    #endif


    client.stop();

    TF_LITE_REPORT_ERROR(error_reporter, "image sent");
 
  } else {
    TF_LITE_REPORT_ERROR(error_reporter, "failed to connect to server");
    return kTfLiteError;
  }

  return kTfLiteOk;
}

void ImageSender::print_wifi_status(tflite::ErrorReporter *error_reporter)
{
  // print the SSID of the network you're attached to:
  char buff[255];
  snprintf(buff, sizeof(buff), "Attached to WiFi SSID: %s", WiFi.SSID());
  TF_LITE_REPORT_ERROR(error_reporter, buff);
}
