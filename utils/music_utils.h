#ifndef MUSIC_H
#define MUSIC_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "music_tbls.h"

#define MIN_TEMPO           80
#define MAX_TEMPO           160
#define PHRASE_COUNT        6     /// amount of phrases in a full song
#define PROGRESSION_LENGTH  4     /// number of chords in a progression
#define PROGRESSION_COUNT   10    /// number of different chord progressions
#define MEASURE_COUNT       4     /// number of measures per phrase

#define MIN_STARTING_KEY_MIDI   57  // NOTE_A3
// MAX_STARTING_KEY_MIDI cannot be higher than 108 (C8) or will overflow octave
#define MAX_STARTING_KEY_MIDI   69  // NOTE_A4

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
  PhraseD = 3,
  NUM_PHRASES
} PhraseID;

/// SONG COMPONENTS
typedef struct chord {
  Note chord_notes[7];
  Note nonchord_notes[7];
  int num_chord_notes;
  int num_nonchord_notes;
  Note key_root; /// this is the root of the key, not of the chord
} chord;

typedef struct aleaNote {
  float freq[3]; /// frequency of note in hz. up to 3 frequencies per struct
  float len; /// length of note--1.0 is a whole note, 0.125 an eight note
  u8 count;  /// amount of notes in freq[] buffer
} aleaNote;

typedef struct aleaMeasure {
  aleaNote m_notes[8];
  aleaNote bassline;
  chord m_chord;
} aleaMeasure;

typedef struct aleaPhrase {
  aleaMeasure m_measures[MEASURE_COUNT];
  int m_progression[PROGRESSION_LENGTH][3];
  Note key_note;
  u8 flags;
} aleaPhrase;

typedef struct aleaSong {
  aleaPhrase m_phrases[4];
  int phrase_dict[4][PROGRESSION_LENGTH][3]; /// mapping of progressions to phrase type
  Note key_note;
  float tempo_bpm;
  PhraseID structure[PHRASE_COUNT];
  u8 flags;
} aleaSong;

/// Initializers
void setSongKey(aleaSong* song);
void setSongTempo(aleaSong* song);
void createSongPattern(aleaSong* song);
void initSongPhrases(aleaSong* song);
void initPhraseMeasures(aleaPhrase* phrase);

/// methods to populate song with notes
void populateMeasure(aleaMeasure* measure, u8 flags);
void chordFromProgression(chord* cchord, int m_chord[3]);

/// Print utilities
char fmtPhraseID(PhraseID ptype);
void printNote(aleaNote* note);
void printMeasure(aleaMeasure* measure);
void printPhrase(aleaPhrase* phrase);
void printSong(aleaSong* song);

#endif
