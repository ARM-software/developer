#include "main_functions.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FLAG_VALUE 123

void launch_core1_entry() {
  printf("Starting Video Model on Core 1\n");
  unsigned int i = 0;
  video_setup();
  while(true) {
    if (i == 2) {
      i = 0;
    }
    //pass the i value to use to core 0
    multicore_fifo_push_blocking(i);
    run_video_model(i);
    i += 1;
  }
  //load video data into video input
  //memcpy(video_input->data.int8, test_video[i], video_input->bytes);
  // invoke / run the model
  //TfLiteStatus video_invoke_status = video_interpreter->Invoke();
  //if (video_invoke_status != kTfLiteOk) {
  //  TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n",
  //                       i);
  //  return;
  //}
  //grab the video score from the output tensor
  //int8_t video_score = video_output->data.int8[0]
  // push the video score to the fifo push blocking
  //multicore_fifo_push_blocking(x);
}

int main(int argc, char* argv[]) {
  //absolute_time_t hundred_loop_start;
  //setup();
  //audio_setup();
  //video_setup();
  stdio_init_all();
  sleep_ms(10000);
  printf("starting script \n");
  sleep_ms(1000);
  ///tag::setup_multicore[]
  multicore_launch_core1(launch_core1_entry);
  // grab the i value to use
  audio_setup();
  //int loop_num = 1;
  while (true) {
    absolute_time_t loop_start = get_absolute_time();
    unsigned int i = multicore_fifo_pop_blocking();
    //printf("grabbed i value %d\n", i);
    run_audio_model(i);

    int64_t time_diff = absolute_time_diff_us(loop_start, get_absolute_time());
    //if (loop_num == 100) {
    //  int64_t hundred_loops_end = absolute_time_diff_us(hundred_loop_start, get_absolute_time());
    //  int64_t loops_average = (hundred_loops_end) / 100;
    //  printf("100 loop average took %d microseconds \n", static_cast<double>(loops_average));
    //}
    printf("Loop took %e microseconds \n", static_cast<double>(time_diff));
    //loop_num += 1;

    //printf("done\n");
  }
  ///end::setup_multicore[]
}
