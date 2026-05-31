#ifndef WAV_H
#define WAV_H

#include <sndfile.h>
#include "portaudio.h"
#include "music_utils.h"

#define SAMPLE_RATE 48000
#define QUARTER_AMPLITUDE 8192
#define HALF_AMPLITUDE 16384

typedef struct wavContainer {
  aleaSong* song;
  s16* wav_data;
  u32 buf_count;
  u32 buf_whole_note;
  u32 iter;
  SF_INFO snd_info;
  SNDFILE* snd_file;
} wavContainer;

void initWavContainer(wavContainer* wav, aleaSong* song);

// write single note to wav data
void write_note(wavContainer* wav, aleaNote* note);

void write_song(wavContainer* wav);

int write_song_to_file(wavContainer* wav, const char *filename);

// play wav data
int wav_playback(wavContainer* wav);

#endif
