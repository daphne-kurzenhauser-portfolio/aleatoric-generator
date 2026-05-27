#include <stdio.h>
#include <stdint.h>
#include <sndfile.h>
#include <math.h>
#include "utils/music_utils.h"
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

int main()
{
  aleaSong song;
  if (!setSongKey(&song)) { 
    fprintf(stderr, "Couldn't set song key\n");
    return 1;
  }
  if (!setSongTempo(&song)) {
    fprintf(stderr, "Couldn't set song key\n");
    return 1;
  }
  if (!createSongPattern(&song)) {
    fprintf(stderr, "Couldn't create song pattern\n");
    return 1;
  }

  printf("INITIALIZING SONG PHRASES\n");
  if (!initSongPhrases(&song)) {
    fprintf(stderr, "Couldn't initialize song phrases\n");
    return 1;
  } else {
    printf("Song phrases initialized\n");
  }

  printSong(&song);

  return 0;
}
