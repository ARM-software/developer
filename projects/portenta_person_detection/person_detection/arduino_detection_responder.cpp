/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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

/*
   Modified 2021-01-20 and 2021-02-16, Rich Freedman, Chariot Solutions, Inc. (https://chariotsolutions.com)
   for ARM (https://arm.com)

   - Use correct LED HIGH and LOW values for the Portenta H7 board
   - Log person recognition
*/

#include "Arduino.h"
#include "detection_responder.h"

// The RGB LEDs on the Arduino Portenta H7
// are on when the pin is LOW, off when HIGH.
#define ON LOW
#define OFF HIGH

// Flash the blue LED after each inference
void RespondToDetection(tflite::ErrorReporter *error_reporter, uint8_t person_score, uint8_t no_person_score)
{
    static bool is_initialized = false;
    if (!is_initialized) {
        // Pins for the built-in RGB LEDs on the Arduino Nano 33 BLE Sense
        pinMode(LEDR, OUTPUT);
        pinMode(LEDG, OUTPUT);
        pinMode(LEDB, OUTPUT);
        is_initialized = true;
    }

    // Switch the person/not person LEDs off
    digitalWrite(LEDG, OFF);
    digitalWrite(LEDR, OFF);

    // Flash the blue LED after every inference.
    digitalWrite(LEDB, ON);
    delay(100);
    digitalWrite(LEDB, OFF);

    // Switch on the green LED when a person is detected,
    // the red when no person is detected
    if (person_score > no_person_score) {
        TF_LITE_REPORT_ERROR(error_reporter, "\nPERSON!: Person score: %d No person score: %d", person_score, no_person_score);
        digitalWrite(LEDG, ON);
        digitalWrite(LEDR, OFF);
    } else {
        TF_LITE_REPORT_ERROR(error_reporter, "\nno person: Person score: %d No person score: %d", person_score, no_person_score);
        digitalWrite(LEDG, OFF);
        digitalWrite(LEDR, ON);
    }
}

