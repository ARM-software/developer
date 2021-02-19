#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_
// Expose a C friendly interface for main functions.
#ifdef __cplusplus
extern "C" {
#endif

// Initializes all data needed for the example. The name is important, and needs
// to be setup() for Arduino compatibility.
void audio_setup();
void video_setup();
void run_video_model(unsigned int i);
void run_audio_model(unsigned int i);
void handle_output(unsigned int i, int audio_score);
//void run_audio_model();
//void run_video_model();
// Runs one iteration of data gathering and inference. This should be called
// repeatedly from the application code. The name needs to be loop() for Arduino
// compatibility.

#ifdef __cplusplus
}
#endif

#endif
