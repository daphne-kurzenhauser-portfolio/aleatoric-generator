#include <stdio.h>
#include <stdint.h>
#include <sndfile.h>
#include <math.h>
#include "portaudio.h"

#define SAMPLE_RATE       48000
#define NUM_SECONDS       1
#define FREQUENCY_HZ      440
#define QUARTER_AMPLITUDE 8192
#define HALF_AMPLITUDE    16384
#define CHANNEL_COUNT     1

// macroing the default SF_INFO struct--48KHz sample rate, mono, 16-bit signed WAV
#define DEFAULT_SND_INFO  {\
  .samplerate = SAMPLE_RATE,\
  .channels   = CHANNEL_COUNT,\
  .format     = SF_FORMAT_WAV | SF_FORMAT_PCM_16 }

typedef int16_t s16;
const double PI = 3.141592653589793;

// simple clamping function
s16 clamp(s16 val, s16 min, s16 max)
{
  const s16 _tmp = val < min ? min : val;
  return _tmp > max ? max : _tmp;
}

int main()
{
  // setting up everything
  s16 sine_data[SAMPLE_RATE * NUM_SECONDS];
  s16 clipped_data[SAMPLE_RATE * NUM_SECONDS];

  SF_INFO sine_wav_info     = DEFAULT_SND_INFO;
  SF_INFO clipped_wav_info  = DEFAULT_SND_INFO;
  SNDFILE* sine_wav_file    = sf_open("sine.wav", SFM_WRITE, &sine_wav_info);
  SNDFILE* clipped_wav_file = sf_open("clipped.wav", SFM_WRITE, &clipped_wav_info);

  for (int i=0; i<(SAMPLE_RATE * NUM_SECONDS); i++)
  {
    // sin takes radians, so we divide 2*pi by 440 to get our angle intervals
    double angle    = (2 * PI / FREQUENCY_HZ) * (i % FREQUENCY_HZ);
    sine_data[i]    = sin(angle) * QUARTER_AMPLITUDE;
    clipped_data[i] = clamp(
        sin(angle) * HALF_AMPLITUDE, 
        (-1 * QUARTER_AMPLITUDE), 
        QUARTER_AMPLITUDE
        );
  }

  sf_writef_short(sine_wav_file, sine_data, SAMPLE_RATE * NUM_SECONDS);
  sf_writef_short(clipped_wav_file, clipped_data, SAMPLE_RATE * NUM_SECONDS);
  sf_close(sine_wav_file);
  sf_close(clipped_wav_file);

  // PortAudio Playback ========================================================
  PaError err = Pa_Initialize();
  if (err != paNoError) goto error;

  // it's necessary to set up a default input channel to let PortAudio stream,
  // but we don't actually need a device assigned--just a non-zero channel count
  // and a matching sample format type
  PaStreamParameters input_params = {
    .channelCount = 1, 
    .sampleFormat = paInt16,
  };
  PaStreamParameters output_params = {
  .device = Pa_GetDefaultOutputDevice(),
  .channelCount = 1,
  .sampleFormat = paInt16,
  };
  output_params.suggestedLatency = Pa_GetDeviceInfo(output_params.device)->defaultHighOutputLatency;

  PaStream *stream;
  err = Pa_OpenStream(
      &stream,
      &input_params, &output_params,
      SAMPLE_RATE, SAMPLE_RATE, // this makes each buffer a full second long
      paClipOff,
      NULL, NULL);
  if (err != paNoError) goto error;

  err = Pa_StartStream(stream);
  if (err != paNoError) goto error;
  printf("Starting stream\n");
  fflush(stdout);

  // just write the whole buffer, it's only one second long
  Pa_WriteStream(stream, &clipped_data, SAMPLE_RATE * NUM_SECONDS);

  err = Pa_StopStream(stream);
  if (err != paNoError) goto error;
  err = Pa_CloseStream(stream);
  if (err != paNoError) goto error;

  Pa_Terminate();
  return 0;

error:
  printf("Error occured with PortAudio: %s\n", Pa_GetErrorText(err));
  Pa_Terminate();
  return 1;
}
