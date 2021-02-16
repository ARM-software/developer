/*
 * Rich Freedman, Chariot Solutions <https://chariotsolutions.com> for ARM
 */
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
