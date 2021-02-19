#include "x_test_audio1.h"
#include "x_test_audio2.h"
#include "audio_model.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Create an area of memory to use for input, output, and intermediate arrays.
constexpr int tensor_arena_size = 5000 * 1024; // probably need smaller
uint8_t tensor_arena[tensor_arena_size];

TF_LITE_MICRO_TESTS_BEGIN

TF_LITE_MICRO_TEST(TestInvoke) {
  // Set up logging.
  tflite::MicroErrorReporter micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = ::tflite::GetModel(audio_model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         model->version(), TFLITE_SCHEMA_VERSION);
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  //tflite::AllOpsResolver micro_op_resolver;
  tflite::MicroMutableOpResolver<6> micro_op_resolver;
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddSoftmax();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddLogistic();
  //tflite::MicroMutableOpResolver<13> micro_op_resolver;
  //micro_op_resolver.AddMaxPool2D();
  //micro_op_resolver.AddConv2D();
  //micro_op_resolver.AddDepthwiseConv2D();
  //micro_op_resolver.AddReshape();
  //micro_op_resolver.AddSoftmax();
  //micro_op_resolver.AddRelu();
  //micro_op_resolver.AddFullyConnected();
  //micro_op_resolver.AddQuantize();
  //micro_op_resolver.AddDequantize();
  //micro_op_resolver.AddPad();
  //micro_op_resolver.AddAdd();
  //micro_op_resolver.AddMean();
  //micro_op_resolver.AddLogistic();
  // Build an interpreter to run the model with.
  tflite::MicroInterpreter interpreter(model, micro_op_resolver, tensor_arena,
                                       tensor_arena_size,
                                       &micro_error_reporter);
  interpreter.AllocateTensors();

  // Get information about the memory area to use for the model's input.
  TfLiteTensor* input = interpreter.input(0);
  // Make sure the input has the properties we expect.
  TF_LITE_MICRO_EXPECT_NE(nullptr, input);
  TF_LITE_MICRO_EXPECT_EQ(4, input->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(16, input->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(16, input->dims->data[2]);
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[3]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, input->type);

  // Copy an audio the memory area used for the input.
  TFLITE_DCHECK_EQ(input->bytes, static_cast<size_t>(x_test_audio1_txt_len));
  //load_data(data_1_txt, input);
  memcpy(input->data.int8, x_test_audio1_txt, input->bytes);

  // Run the model on this input and make sure it succeeds.
  TfLiteStatus invoke_status = interpreter.Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(&micro_error_reporter, "Invoke failed\n");
  }
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // Get the output from the model, and make sure it's the expected size and
  // type.
  TfLiteTensor* output = interpreter.output(0);
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[1]);
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, output->type);


  TF_LITE_REPORT_ERROR(&micro_error_reporter, "Ran successfully\n");
}

TF_LITE_MICRO_TESTS_END
