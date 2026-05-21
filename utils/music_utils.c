#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "music_tbls.h"
#include "music_utils.h"

const int progressions[PROGRESSION_COUNT][PROGRESSION_LENGTH][3] = {
  {chord_I,chord_IV,chord_ii,chord_V},
  {chord_I,chord_vi,chord_ii,chord_V},
  {chord_I,chord_iii,chord_IV,chord_iv},
  {chord_I,chord_V,chord_ii,chord_V},
  {chord_I,chord_vi,chord_IV,chord_V},
  {chord_IV,chord_I,chord_vi,chord_IV},
  {chord_I,chord_V,chord_vi,chord_I},
  {chord_I,chord_IV,chord_iv,chord_I},
  {chord_IV,chord_V,chord_I,chord_I},
  {chord_vi,chord_IV,chord_I,chord_V}
};

const int MAJOR_SCALE_INDICES[7] = SCALE_MAJ;

const PhraseID songStructures[][PHRASE_COUNT] = {
  {PhraseA, PhraseA, PhraseB, PhraseB, PhraseC, PhraseC},
  {PhraseA, PhraseB, PhraseA, PhraseB, PhraseC, PhraseD},
  {PhraseA, PhraseB, PhraseC, PhraseD, PhraseD, PhraseD}
};

const float key_octaves[13][13] = {
  A3_oct,
  Bb3_oct,
  B3_oct,
  C4_oct,
  Db4_oct,
  D4_oct,
  Eb4_oct,
  E4_oct,
  F4_oct,
  Fs4_oct,
  G4_oct,
  Ab4_oct,
  A4_oct
};

bool setSongKey(aleaSong* song)
{
  srand(time(NULL));
  int _keyID = rand() % 13;
  memcpy(song->octave, key_octaves[_keyID], sizeof(key_octaves[_keyID]));
  return true;
}

bool setSongTempo(aleaSong* song)
{
  srand(time(NULL));
  song->tempo_bpm = (rand() % MIN_TEMPO) + (MAX_TEMPO - MIN_TEMPO);
  return true;
}

bool createSongPattern(aleaSong* song)
{
  int _phraseID = rand() % 3;
  int prog_A = rand() % PROGRESSION_COUNT;
  int prog_B = rand() % PROGRESSION_COUNT;
  int prog_C = rand() % PROGRESSION_COUNT;
  int prog_D = rand() % PROGRESSION_COUNT;

  memcpy(song->structure, songStructures[_phraseID], sizeof(songStructures[_phraseID]));

  while (prog_A == prog_B) { 
    prog_B = rand() % PROGRESSION_COUNT;
  }
  while ((prog_C == prog_B) || (prog_C == prog_A)) {
    prog_C = rand() % PROGRESSION_COUNT;
  }
  while ((prog_D == prog_C) || (prog_D == prog_B) || (prog_D == prog_A)) {
    prog_D = rand() % PROGRESSION_COUNT;
  }

  memcpy(song->phrase_dict[0], progressions[prog_A], sizeof(progressions[prog_A]));
  memcpy(song->phrase_dict[1], progressions[prog_B], sizeof(progressions[prog_B]));
  memcpy(song->phrase_dict[2], progressions[prog_C], sizeof(progressions[prog_C]));
  memcpy(song->phrase_dict[3], progressions[prog_D], sizeof(progressions[prog_D]));

  return true;
}

bool initSongPhrases(aleaSong* song)
{
  for (int i=0; i<PHRASE_COUNT; i++) {
    PhraseID _phraseID = song->structure[i];
    aleaPhrase* phrase = &(song->m_phrases[i]);
    phrase->octave = &(song->octave);
    memcpy(phrase->m_progression, song->phrase_dict[_phraseID], 
        sizeof(song->phrase_dict[_phraseID]));
    initPhraseMeasures(phrase);
  }
  return true;
}

bool initPhraseMeasures(aleaPhrase* phrase)
{
  for (int i=0; i<MEASURE_COUNT; i++) {
    aleaMeasure* measure = &(phrase->m_measures[i]);
  }
}

bool chordFromProgression(chord* cchord, float (*octave)[13], int m_chord[3])
{
  cchord->num_chord_notes = 0;
  cchord->num_nonchord_notes = 0;
  for (int i=1; i<13; i++) {
    for (int j=0; j<3; j++) {
      if (i == m_chord[j]) {
        cchord->chord_notes[num_chord_notes] = i;
        num_chord_notes++;
        break;
      }
    }
    for (int j=0; j<7; j++) {
      if (i == MAJOR_SCALE_INDICES[j]) {
        cchord->chord_notes[num_chord_notes] = i;
        num_chord_notes++;
        break;
      }
    }
    cchord->nonchord_notes[num_nonchord_notes] = i;
    num_nonchord_notes++;
  }
}
