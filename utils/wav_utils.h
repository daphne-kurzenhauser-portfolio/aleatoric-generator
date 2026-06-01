#ifndef WAV_H
#define WAV_H

#include <sndfile.h>
#include "portaudio.h"
#include "music_utils.h"

#define SAMPLE_RATE 48000
#define EIGHTH_AMPLITUDE 4096.0f
#define QUARTER_AMPLITUDE 8192.0f
#define HALF_AMPLITUDE 16384.0f
#define ATTACK_ENVELOPE_MS  25.0f
#define RELEASE_ENVELOPE_MS 25.0f

typedef struct wavContainer {
  aleaSong* song;
  s16* wav_data;
  u32 buf_count;
  u32 buf_whole_note;
  u32 iter;
  SF_INFO snd_info;
  SNDFILE* snd_file;
  u8 flags;
} wavContainer;

void initWavContainer(wavContainer* wav, aleaSong* song);

// write single note to wav data
u32 write_note(wavContainer* wav, aleaNote* note);

void write_song(wavContainer* wav);

int write_song_to_file(wavContainer* wav, const char *filename);

// play wav data
int wav_playback(wavContainer* wav);

#endif
