#ifndef MUSIC_H
#define MUSIC_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define MIN_TEMPO           80
#define MAX_TEMPO           160
#define PHRASE_COUNT        6
#define PROGRESSION_LENGTH  4
#define PROGRESSION_COUNT   10
#define MEASURE_COUNT       4

#define FLAG_BASS     0x01
#define FLAG_HARMONY  0x02
#define FLAG_RHYTHM   0x04
#define FLAG_DRUMS    0x08
#define FLAG_MIDI     0x10
#define FLAG_OUTPUT   0x20

typedef uint8_t u8;
typedef int16_t s16;
typedef uint32_t u32;

typedef enum {
  PhraseA = 0,
  PhraseB = 1,
  PhraseC = 2,
  PhraseD = 3
} PhraseID;

/// SONG COMPONENTS
typedef struct chord {
  float chord_notes[7];
  float nonchord_notes[7];
  int num_chord_notes;
  int num_nonchord_notes;
} chord;

typedef struct aleaNote {
  float freq[3]; // frequency of note in hz. dynamically-allocated for multiple notes
  float len; // length of note--1.0 is a whole note, 0.125 an eight note
  u8 count;  // length of *freq buffer
} aleaNote;

typedef struct aleaMeasure {
  aleaNote m_notes[8];
  chord m_chord;
} aleaMeasure;

typedef struct aleaPhrase {
  aleaMeasure m_measures[MEASURE_COUNT];
  int m_progression[PROGRESSION_LENGTH][3];
  float (*octave)[13];
} aleaPhrase;

typedef struct aleaSong {
  aleaPhrase m_phrases[4];
  int phrase_dict[4][PROGRESSION_LENGTH][3];
  float octave[13];
  float tempo_bpm;
  PhraseID structure[PHRASE_COUNT];
} aleaSong;

void setSongKey(aleaSong* song);
void setSongTempo(aleaSong* song);
void createSongPattern(aleaSong* song);
int initSongPhrases(aleaSong* song);

int initPhraseMeasures(aleaPhrase* phrase);

int populateMeasure(aleaMeasure* measure);

int chordFromProgression(chord* cchord, float (*octave)[13], int m_chord[3]);

char fmtPhraseID(PhraseID ptype);
void printNote(aleaNote* note);
void printMeasure(aleaMeasure* measure);
void printPhrase(aleaPhrase* phrase);
void printSong(aleaSong* song);

#endif
