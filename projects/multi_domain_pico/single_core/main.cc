#include "main_functions.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]) {
  //absolute_time_t hundred_loop_start;
  stdio_init_all();

  sleep_ms(10000);
  printf("starting script \n");
  sleep_ms(1000);

  audio_setup();
  video_setup();

  unsigned int i = 0;
  //int loop_num = 1;

  while (true) {
    //if (loop_num == 101) {
    //  loop_num = 1;
    //}
    absolute_time_t loop_start = get_absolute_time();
    //if (loop_num == 1) {
    //  hundred_loop_start = get_absolute_time();
    //}
    if (i == 2) {
      i = 0;
    }

    loop(i);

    i+= 1;

    //get time taken
    int64_t time_diff = absolute_time_diff_us(loop_start, get_absolute_time());
    //if (loop_num == 100) {
    //  int64_t hundred_loops_end = absolute_time_diff_us(hundred_loop_start, get_absolute_time());
    //  int64_t loops_average = (hundred_loops_end) / 100;
    //  printf("100 loop average took %d microseconds \n", static_cast<double>(loops_average));
    //}
    printf("Loop took %e microseconds \n", static_cast<double>(time_diff));
    //loop_num += 1;
  }
}
