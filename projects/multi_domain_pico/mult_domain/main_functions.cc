#include "main_functions.h"
//#include "video_model.h"
#include "video_model_mobilenet.h"
#include "audio_model.h"
#include "x_test_audio1.h"
#include "x_test_audio2.h"
#include "x_test_video1.h"
#include "x_test_video2.h"

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "pico/stdlib.h"
#include <stdint.h>
#include <string.h>
//#include <stdint.h>
#include "pico/multicore.h"
#include <stdio.h>

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
//tflite::ErrorReporter* audio_error_reporter = nullptr;
const tflite::Model* audio_model = nullptr;
tflite::MicroInterpreter* audio_interpreter = nullptr;
TfLiteTensor* audio_input = nullptr;
TfLiteTensor* audio_output = nullptr;

constexpr int audio_tensor_arena_size = 30 * 1024; // probably need smaller
uint8_t audio_tensor_arena[audio_tensor_arena_size];

const unsigned char* test_audio[] = {
  x_test_audio1_txt, x_test_audio2_txt
};

//tflite::ErrorReporter* video_error_reporter = nullptr;
const tflite::Model* video_model = nullptr;
tflite::MicroInterpreter* video_interpreter = nullptr;
TfLiteTensor* video_input = nullptr;
TfLiteTensor* video_output = nullptr;

constexpr int video_tensor_arena_size = 70 * 1024; // probably need smaller
uint8_t video_tensor_arena[video_tensor_arena_size];

const unsigned char* test_video[] = {
  x_test_video1_txt, x_test_video2_txt
};

int i = 0;
}  // namespace

void audio_setup() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  audio_model = tflite::GetModel(audio_model_tflite);
  if (audio_model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         audio_model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::MicroMutableOpResolver<6> audio_micro_op_resolver;
  audio_micro_op_resolver.AddMaxPool2D();
  audio_micro_op_resolver.AddConv2D();
  audio_micro_op_resolver.AddReshape();
  audio_micro_op_resolver.AddSoftmax();
  audio_micro_op_resolver.AddFullyConnected();
  audio_micro_op_resolver.AddLogistic();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter audio_static_interpreter(
    audio_model, audio_micro_op_resolver, audio_tensor_arena,
    audio_tensor_arena_size, &micro_error_reporter);
  audio_interpreter = &audio_static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus audio_allocate_status = audio_interpreter->AllocateTensors();
  if (audio_allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  audio_input = audio_interpreter->input(0);
  audio_output = audio_interpreter->output(0);

}

void video_setup() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  video_model = tflite::GetModel(video_model_mobilenet_tflite);
  if (video_model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         video_model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::MicroMutableOpResolver<7> video_micro_op_resolver;
  video_micro_op_resolver.AddConv2D();
  video_micro_op_resolver.AddReshape();
  video_micro_op_resolver.AddFullyConnected();
  video_micro_op_resolver.AddLogistic();
  video_micro_op_resolver.AddMul();
  video_micro_op_resolver.AddAdd();
  video_micro_op_resolver.AddMaxPool2D();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter video_static_interpreter(
    video_model, video_micro_op_resolver, video_tensor_arena,
    video_tensor_arena_size, &micro_error_reporter);
  video_interpreter = &video_static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus video_allocate_status = video_interpreter->AllocateTensors();
  if (video_allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  video_input = video_interpreter->input(0);
  video_output = video_interpreter->output(0);

}

void run_video_model(unsigned int i) {
  memcpy(video_input->data.int8, test_video[i], video_input->bytes);

  TfLiteStatus video_invoke_status = video_interpreter->Invoke();
  if (video_invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n",
                         i);
    return;
  }
  int8_t video_score = video_output->data.int8[0];

  multicore_fifo_push_blocking(static_cast<uint32_t>(video_score));
  //printf("video_score: %f\n", static_cast<double>(video_score));
}

void run_audio_model(unsigned int i) {
  memcpy(audio_input->data.int8, test_audio[i], audio_input->bytes);

  TfLiteStatus audio_invoke_status = audio_interpreter->Invoke();
  if (audio_invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n",
                         i);
    return;
  }
  int8_t audio_score = audio_output->data.int8[0];
  //printf("audio_score: %f\n", static_cast<double>(audio_score));
  handle_output(i, audio_score);
}

void handle_output(unsigned int i, int audio_score) {
  static bool is_initialized = false;
  static bool toggle = false;
  const uint LED_PIN = 25;

  if(!is_initialized) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    is_initialized = true;
  }
  unsigned int video_score = multicore_fifo_pop_blocking();

  if(!toggle) {
    gpio_put(LED_PIN, 1);
    toggle = true;
  } else {
      gpio_put(LED_PIN, 0);
      toggle = false;
  }

  int signed_score = (int)video_score;
  printf("audio score = %f, video score = %f from i =  %f\n",
        static_cast<double>(audio_score), static_cast<double>(signed_score),
        static_cast<double>(i));
  int average = (audio_score + signed_score) / 2;
  printf("total score %d\n", static_cast<double>(average));

}
