/*
   Modified 2021-01-20 and 2021-02-16, Rich Freedman, Chariot Solutions, Inc. (https://chariotsolutions.com)
   for ARM (https://arm.com)

   - Make the code work with the Arduino Portenta H7 board and Portenta Vision Shield
   - Get image from camera at 320x240 (since it's the only size that's currently supported by the CameraClass)
   - Update to work with Arduino/Mbed library version 1.3.2
*/

#include <string.h>

#include "Arduino.h"        // required for use of camera.h in .cpp file
#include "camera.h"         // header for the CameraClass class
#include "image_provider.h" // the interface that is implemented here

#define SUPPORTED_IMAGE_WIDTH 320
#define SUPPORTED_IMAGE_HEIGHT 240

// Camera  instance
CameraClass cam;

static bool g_is_camera_initialized = false;


// Get the camera module ready
TfLiteStatus InitCamera(tflite::ErrorReporter* error_reporter, int image_width, int image_height) {

    /**
     * RAF: This is odd, because the CameraClass allows you to pass pretty much any value for IMAGE_WIDTH and IMAGE_HEIGHT,
     * but as currently implemented, only actually works with a single resolution.
     */
     if(image_width != SUPPORTED_IMAGE_WIDTH || image_height != SUPPORTED_IMAGE_HEIGHT) {
      TF_LITE_REPORT_ERROR(
        error_reporter, 
        "arduino_image_provider InitCamera: only currently supported resolution is %dx%d, got %dx%d", 
        SUPPORTED_IMAGE_WIDTH, SUPPORTED_IMAGE_HEIGHT, image_width, image_height
      );
      return kTfLiteError;
    }

    // Initialize the camera for QVGA, 30FPS
    int begin_status = cam.begin(CAMERA_R320x240, 30);
    if (begin_status != 0) {
        TF_LITE_REPORT_ERROR(error_reporter, "arduino_image_provider InitCamera: camera.begin() failed with status %d", begin_status);
        return kTfLiteError;
    }

    return kTfLiteOk;
}


// Get an image from the camera module
TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width, int image_height, int channels, uint8_t* image_data) {

    if (!g_is_camera_initialized) {
         TfLiteStatus init_status = InitCamera(error_reporter, image_width, image_height);
        if (init_status != kTfLiteOk) {
            TF_LITE_REPORT_ERROR(error_reporter, "GetImage - InitCamera failed");
            return init_status;
        }
        g_is_camera_initialized = true;
    }

    if(cam.grab(image_data) != 0) {
        TF_LITE_REPORT_ERROR(error_reporter, "GetImage  - grab failed");
        return kTfLiteError;
    }

    return kTfLiteOk;
}
