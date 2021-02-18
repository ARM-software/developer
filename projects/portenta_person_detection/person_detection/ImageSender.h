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

#ifndef PERSON_DETECTION_IMAGESENDER_H
#define PERSON_DETECTION_IMAGESENDER_H

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

class ImageSender
{
    private: 
        void print_wifi_status(tflite::ErrorReporter* error_reporter);

    public:
        TfLiteStatus connect_to_wifi(tflite::ErrorReporter* error_reporter);
        TfLiteStatus send_image(tflite::ErrorReporter* error_reporter, const uint8_t *image, int width, int height, int person_score, int no_person_score);
};


#endif //PERSON_DETECTION_IMAGESENDER_H
