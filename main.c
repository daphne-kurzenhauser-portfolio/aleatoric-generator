#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include <math.h>
#include "utils/music_utils.h"
#include "utils/wav_utils.h"
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

int main(int argc, char *argv[])
{
  aleaSong song;
  wavContainer wav;
  u8 FLAGS = 0;
  char filename[32];

  for (int i=1; i<argc; i++) {
    const char *arg = argv[i];
    if (strcmp(arg, "--help") == 0) {
      printf("HELP REQUESTED\n");
    } else if (strcmp(arg, "--bass") == 0) {
      printf("BASS selected\n");
      FLAGS |= FLAG_BASS;
    } else if (strcmp(arg, "--harmony") == 0) {
      printf("HARMONY selected\n");
      FLAGS |= FLAG_HARMONY;
    } else if (strcmp(arg, "--rhythm") == 0) {
      printf("RHYTHM selected\n");
      FLAGS |= FLAG_RHYTHM;
    } else if (strcmp(arg, "--drums") == 0) {
      printf("DRUMS selected\n");
      FLAGS |= FLAG_DRUMS;
    } else if (strcmp(arg, "--midi") == 0) {
      printf("MIDI selected\n");
      FLAGS |= FLAG_MIDI;
    } else if (strcmp(arg, "--output") == 0) {
      if ((i+1) == argc) {
        fprintf(stderr, "must supply filename to write to for output\n");
        return 0;
      }
      strncpy(filename, argv[i+1], sizeof(filename));
      printf("OUTPUT selected: %s\n", filename);
      FLAGS |= FLAG_OUTPUT;
    }
  }

  song.flags = FLAGS;
  setSongKey(&song);
  setSongTempo(&song);
  createSongPattern(&song);

  if (!initSongPhrases(&song)) {
    fprintf(stderr, "Couldn't initialize song phrases\n");
    return 1;
  }
  printf("Song phrases initialized\n");

  initWavContainer(&wav, &song);
  write_song(&wav);

  if ((FLAGS & FLAG_OUTPUT) == (FLAG_OUTPUT)) {
    printf("Writing output to %s\n", filename);
    write_song_to_file(&wav, filename);
  }

  wav_playback(&wav);

  printf("Exiting!\n");

  return 0;
}
