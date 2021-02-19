#include "x_test_video1.h"
#include "x_test_video2.h"
#include "x_test_audio1.h"
#include "x_test_audio2.h"
#include "audio_model.h"
#include "video_model.h"
#include "video_model_mobilenet.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Create an area of memory to use for input, output, and intermediate arrays.
constexpr int tensor_arena_size1 = 20 * 1024; // probably need smaller
uint8_t tensor_arena1[tensor_arena_size1];
constexpr int tensor_arena_size2 = 70 * 1024; // probably need smaller
uint8_t tensor_arena2[tensor_arena_size2];

TF_LITE_MICRO_TESTS_BEGIN

TF_LITE_MICRO_TEST(TestInvoke) {
  // Set up logging.
  tflite::MicroErrorReporter micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* audio_model = ::tflite::GetModel(audio_model_tflite);
  if (audio_model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         audio_model->version(), TFLITE_SCHEMA_VERSION);
  }
  const tflite::Model* video_model = ::tflite::GetModel(video_model_mobilenet_tflite);
  if (video_model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         video_model->version(), TFLITE_SCHEMA_VERSION);
  }


  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  tflite::MicroMutableOpResolver<13> micro_op_resolver;
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddSoftmax();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddLogistic();
  micro_op_resolver.AddDepthwiseConv2D();
  micro_op_resolver.AddAveragePool2D();
  micro_op_resolver.AddQuantize();
  micro_op_resolver.AddDequantize();
  micro_op_resolver.AddConcatenation();
  micro_op_resolver.AddMul();
  micro_op_resolver.AddAdd();
  // Build an interpreter to run the model with.
  tflite::MicroInterpreter interpreter1(audio_model, micro_op_resolver, tensor_arena1,
                                       tensor_arena_size1,
                                       &micro_error_reporter);
  interpreter1.AllocateTensors();

  tflite::MicroInterpreter interpreter2(video_model, micro_op_resolver, tensor_arena2,
                                       tensor_arena_size2,
                                       &micro_error_reporter);
  interpreter2.AllocateTensors();


  // Get information about the memory area to use for the model's input.
  TfLiteTensor* audio_input = interpreter1.input(0);
  TfLiteTensor* video_input = interpreter2.input(0);
  // Make sure the input has the properties we expect.
  TF_LITE_MICRO_EXPECT_NE(nullptr, audio_input);
  TF_LITE_MICRO_EXPECT_EQ(4, audio_input->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, audio_input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(16, audio_input->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(16, audio_input->dims->data[2]);
  TF_LITE_MICRO_EXPECT_EQ(1, audio_input->dims->data[3]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, audio_input->type);

  TF_LITE_MICRO_EXPECT_NE(nullptr, video_input);
  TF_LITE_MICRO_EXPECT_EQ(4, video_input->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, video_input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(28, video_input->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(28, video_input->dims->data[2]);
  TF_LITE_MICRO_EXPECT_EQ(1, video_input->dims->data[3]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, video_input->type);

  // Copy an audio the memory area used for the input.
  TFLITE_DCHECK_EQ(audio_input->bytes, static_cast<size_t>(x_test_audio1_txt_len));
  //load_data(data_1_txt, input);
  memcpy(audio_input->data.int8, x_test_audio1_txt, audio_input->bytes);

  // Copy a video screen the memory area used for the input.
  TFLITE_DCHECK_EQ(video_input->bytes, static_cast<size_t>(x_test_video1_txt_len));
  //load_data(data_1_txt, input);
  memcpy(video_input->data.int8, x_test_video1_txt, video_input->bytes);

  // Run the model on this input and make sure it succeeds.
  TfLiteStatus invoke_status = interpreter1.Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter, "Invoke failed\n");
  }
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  invoke_status = interpreter2.Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter, "Invoke failed\n");
  }
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);


  //RUN EACH MODEL

  // happy run

  // get output from audio model
  TfLiteTensor* output1 = interpreter1.output(0);
  TF_LITE_MICRO_EXPECT_EQ(2, output1->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output1->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(1, output1->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, output1->type);

  int8_t happy_audio_score = output1->data.int8[0];

  // get output from video model
  TfLiteTensor* output2 = interpreter2.output(0);
  TF_LITE_MICRO_EXPECT_EQ(2, output2->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output2->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(1, output2->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, output2->type);

  int8_t happy_video_score = output2->data.int8[0];

  //angry run

  memcpy(audio_input->data.int8, x_test_audio2_txt, audio_input->bytes);
  memcpy(video_input->data.int8, x_test_video2_txt, video_input->bytes);

  invoke_status = interpreter1.Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter, "Invoke failed\n");
  }
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  invoke_status = interpreter2.Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter, "Invoke failed\n");
  }
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // get output from audio model
  output1 = interpreter1.output(0);
  TF_LITE_MICRO_EXPECT_EQ(2, output1->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output1->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(1, output1->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, output1->type);

  int8_t angry_audio_score = output1->data.int8[0];

  // get output from video model
  output2 = interpreter2.output(0);
  TF_LITE_MICRO_EXPECT_EQ(2, output2->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output2->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(1, output2->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, output2->type);

  int8_t angry_video_score = output2->data.int8[0];

  // full score = audio + video
  int8_t happy_score = (happy_audio_score + happy_video_score) / 2;
  int8_t angry_score = (angry_audio_score + angry_video_score) / 2;

  TF_LITE_MICRO_EXPECT_GT(happy_audio_score, angry_audio_score);
  printf("%d", happy_audio_score);
  printf("%d", angry_video_score);
  TF_LITE_MICRO_EXPECT_GT(happy_video_score, angry_video_score);
  TF_LITE_MICRO_EXPECT_GT(happy_score, angry_score);

  TF_LITE_REPORT_ERROR(&micro_error_reporter, "Ran successfully\n");
}

TF_LITE_MICRO_TESTS_END
