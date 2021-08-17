#include <TensorFlowLite.h>

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

   - Make the code work with the Arduino Portenta H7 board and Portenta Vision Shield
     - Get image from camera at 320x240 (since it's the only size that works reliably right now)
     - Crop 320x240 image to square (240x240)
     - Scale 240x240 image to 96x96 as expected by TensorFlow

   - Optionally write images to the SD Card (kinda broken - often crashes after a few writes)
   - Optionally HTTP Post images to a server for display
   - Optionally log non-errors ("info") to the serial console
   - Update to work with Arduino/Mbed library version 1.3.2
*/

#include <TensorFlowLite.h>

#include "main_functions.h"

#include "detection_responder.h"
#include "image_provider.h"
#include "model_settings.h"
#include "person_detect_model_data.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

#include "ImageCropper.h"
#include "ImageScaler.h"
#include "ImageSender.h"

int image_count = 0; // affects tensor_arena alignment, but doesn't behave properly inside namespace

// Globals, used for compatibility with Arduino-style sketches.
namespace {

    /** Declare no variables before this, so as not to affect the tensor_arena alignment **/
    // tensor_arena - An area of memory to use for input, output, and intermediate arrays.
    // Requires 16 byte alignment - adjust tensor_arena_alignment_shift as necessary if you get an alignment warning at runtime
    constexpr int kTensorArenaSize = 93 * 1024;
    constexpr size_t tensor_arena_alignment_shift = 4;
    constexpr size_t tensor_arena_padded_size = kTensorArenaSize + tensor_arena_alignment_shift;
    static uint8_t tensor_arena[tensor_arena_padded_size];
    uint8_t *p_tensor_arena = tensor_arena + tensor_arena_alignment_shift;

    tflite::ErrorReporter* error_reporter = nullptr;
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;

    constexpr bool use_sd_card = false;
    constexpr bool write_scaled_image = false;
    constexpr bool send_scaled_image = true;
    constexpr bool write_serial_info = true; // controls "info" output only - errors will always be written
    constexpr int images_to_write = 5;

    // SD Card block device and filesystem
    SDMMCBlockDevice block_device;
    mbed::FATFileSystem fs("fs");

    constexpr int pd_large_image_width = 320;
    constexpr int pd_large_image_height = 240;
    constexpr int pd_large_image_size = (pd_large_image_width * pd_large_image_height);
    constexpr int pd_cropped_dimension = 240;
    constexpr int pd_cropped_size = pd_cropped_dimension * pd_cropped_dimension;

    uint8_t largeImage[pd_large_image_size];
    uint8_t croppedImage[pd_cropped_size];
    uint8_t scaledImage[kMaxImageSize];

    boolean sd_card_initialized = false;
    boolean wifi_initialized = false;

    char filename[255];

    ImageCropper image_cropper;
    ImageScaler image_scaler;
    ImageSender image_sender;

}  // namespace


boolean init_sd_card()
{
  delay(2000);

  int err =  fs.mount(&block_device);
  if (err) {
    // Reformat if we can't mount the filesystem
    // this should only happen on the first boot
    Serial.println("formatting sd card");
    err = fs.reformat(&block_device);

    if(err) {
      Serial.print("failed to mount or reformat file system - error code = ");
      Serial.println(err);
      return false;
    }

    Serial.println("init_sd_card done formatting sd card");
  }

  return true;
}


void write_image_to_sd_card(const char* image_type, const uint8_t* image_data, int imageWidth, int imageHeight, int imageSize, int imageNum, const char *recognition)
{
    // form the file name
    sprintf(filename, "/fs/image_%s_%d_%dx%d_%d_%s.raw", image_type, imageNum, imageWidth, imageHeight, imageSize, recognition);

    if(write_serial_info) {
        Serial.print("writing ");
        Serial.print(imageSize);
        Serial.print(" bytes to SD card as ");
        Serial.println(filename);
    }

    FILE *file = fopen(filename, "wb");
    if(file != NULL) {
        size_t chunk_size = 512;
        size_t total_bytes_written = 0;
        while(total_bytes_written < (size_t) imageSize) {
             size_t write_size = min(chunk_size, imageSize - total_bytes_written);
             size_t bytes_written = fwrite(image_data + total_bytes_written, 1, write_size, file);
             total_bytes_written += bytes_written;
             if(bytes_written == 0)  {
                fclose(file);
                Serial.print("failed to write file ");
                Serial.println(filename);
                return;
             }
             if(bytes_written != write_size) {
                Serial.print("short write - attempted ");
                Serial.print(chunk_size);
                Serial.print(", wrote only ");
                Serial.println(bytes_written);
             }

             delay(20);
        }

        fclose(file);

        if(write_serial_info) {
            Serial.print("wrote ");
            Serial.print(total_bytes_written);
            Serial.println(" bytes to SD Card");
        }
    } else {
        Serial.print("fopen failed with error ");
        Serial.println(errno);
    }
}


void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("ARM Portenta H7 TensorFlow Person Detection Demo\n");

  if(use_sd_card && write_serial_info) {
      Serial.println("Initializing SD card...");
  }

  if(use_sd_card) {
      if (!init_sd_card()) {
          Serial.println("SD card initialization failed, won't write images");
      } else {
          sd_card_initialized = true;

          if(write_serial_info) {
              Serial.println("SD card initialization done. Images will be written");
          }
      }
  }


  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_person_detect_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
     "Model provided is schema version %d not equal "
     "to supported version %d.", model->version(), TFLITE_SCHEMA_VERSION
    );
    return;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  //
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver<3> micro_op_resolver;
  micro_op_resolver.AddDepthwiseConv2D();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddAveragePool2D();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(model, micro_op_resolver, p_tensor_arena, kTensorArenaSize, error_reporter);
  
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  input = interpreter->input(0);

  if(send_scaled_image) {
      if(write_serial_info) {
          Serial.println("connecting to wifi");
      }

      if (image_sender.connect_to_wifi(error_reporter) == kTfLiteOk) {
          wifi_initialized = true;

          if(write_serial_info) {
              Serial.println("wifi connected, will attempt to send images to server");
          }
      } else {
          Serial.println("wifi connection failed, will NOT attempt to send images to server");
      }
  }

}

void loop()
{
  if(write_serial_info) {
      Serial.println();
      Serial.println("==============================");
      Serial.println();
      Serial.print("Getting image ");
      Serial.print(image_count);
      Serial.print(" (");
      Serial.print(pd_large_image_width);
      Serial.print("x");
      Serial.print(pd_large_image_height);
      Serial.println(") from camera");
  }

  // get 'large' image from the provider
  if (kTfLiteOk != GetImage(error_reporter, pd_large_image_width, pd_large_image_height, kNumChannels, largeImage)) {
    Serial.println("Image capture failed.");
    return;
  }

  // crop the image to the square aspect ratio that the model expects (will crop to center)
  if(write_serial_info) {
      Serial.print("Cropping image to ");
      Serial.print(pd_cropped_dimension);
      Serial.print("x");
      Serial.println(pd_cropped_dimension);
  }

  image_cropper.crop_image(largeImage, pd_large_image_width, pd_large_image_height, croppedImage, pd_cropped_dimension, pd_cropped_dimension);


  // scale the image to the size that the model expects (96x96)
  if(write_serial_info) {
      Serial.print("Scaling image to ");
      Serial.print(kNumCols);
      Serial.print("x");
      Serial.println(kNumRows);
  }
  int scale_result = image_scaler.scale_image_down(croppedImage, pd_cropped_dimension, pd_cropped_dimension, scaledImage, kNumRows, kNumCols);
  if(scale_result < 0) {
      Serial.println("Failed to scale image");
      return;
  }

  // copy the scaled image to the TF input
  if(write_serial_info) {
      Serial.println("Copying scaled image to TensorFlow model");
  }
  memcpy(input->data.uint8, scaledImage, kMaxImageSize);

  // Run the model on this input and make sure it succeeds.
  if(write_serial_info) {
      Serial.println("Invoking the TensorFlow interpreter");
  }

  if (kTfLiteOk != interpreter->Invoke()) {
    Serial.println("TensorFlow Invocation failed.");
    return;
  }

  TfLiteTensor* output = interpreter->output(0);

  // Process the inference results
  uint8_t person_score = output->data.uint8[kPersonIndex];
  uint8_t no_person_score = output->data.uint8[kNotAPersonIndex];
  RespondToDetection(error_reporter, person_score, no_person_score);

  // write out the scaled image to the SD Card if present
  if(write_scaled_image && sd_card_initialized && image_count < images_to_write) {
     if(write_serial_info) {
          Serial.print("Writing scaled image ");
          Serial.print(image_count);
          Serial.println(" to sd card");
      }

      write_image_to_sd_card("scaled", scaledImage, kNumRows, kNumCols, kMaxImageSize, image_count, person_score > no_person_score ? "PERSON" : "NOPERSON");
  }

  if(send_scaled_image && wifi_initialized) {
      if(write_serial_info) {
          Serial.print("Sending image ");
          Serial.print(image_count);
          Serial.println(" to server");
      }

      // To send the 96x96 image, if desired
      // TfLiteStatus sendStatus = image_sender.send_image(error_reporter, scaledImage, kNumRows, kNumCols, person_score, no_person_score);

      // send the 240x240 image to the server
      TfLiteStatus sendStatus = image_sender.send_image(error_reporter, croppedImage, pd_cropped_dimension, pd_cropped_dimension, person_score, no_person_score);
      if (sendStatus != kTfLiteOk) {
          Serial.println("image send failed");
      }
  }

  image_count++;
}
