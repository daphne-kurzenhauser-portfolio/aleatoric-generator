#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define MIN_TEMPO           80
#define MAX_TEMPO           160
#define PHRASE_COUNT        6
#define PROGRESSION_LENGTH  4
#define PROGRESSION_COUNT   10
#define MEASURE_COUNT       4

typedef uint8_t u8;
typedef int16_t s16;

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
  float* freq; // frequency of note in hz. dynamically-allocated for multiple notes
  float len; // length of note--1.0 is a whole note, 0.125 an eight note
  u8 count;  // length of *freq buffer
} aleaNote;

typedef struct aleaMeasure {
  aleaNote* m_notes;
  chord m_chord;
} aleaMeasure;

typedef struct aleaPhrase {
  aleaMeasure m_measures[MEASURE_COUNT];
  int m_progression[PROGRESSION_LENGTH][3];
  float (*octave)[13];
} aleaPhrase;

typedef struct aleaSong {
  aleaPhrase m_phrases[PHRASE_COUNT];
  int phrase_dict[4][PROGRESSION_LENGTH][3];
  float octave[13];
  int tempo_bpm;
  PhraseID structure[PHRASE_COUNT];
} aleaSong;

bool setSongKey(aleaSong* song);
bool setSongTempo(aleaSong* song);
bool createSongPattern(aleaSong* song);
bool initSongPhrases(aleaSong* song);

bool initPhraseMeasures(aleaPhrase* phrase);

bool populateMeasure(aleaMeasure* measure);

bool chordFromProgression(chord* cchord, float (*octave)[13], int m_chord[3]);
