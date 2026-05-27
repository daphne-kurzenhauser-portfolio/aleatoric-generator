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

int setSongKey(aleaSong* song)
{
  srand(time(NULL));
  int _keyID = rand() % 13;
  memcpy(song->octave, key_octaves[_keyID], sizeof(key_octaves[_keyID]));
  return 1;
}

int setSongTempo(aleaSong* song)
{
  srand(time(NULL));
  song->tempo_bpm = (rand() % MIN_TEMPO) + (MAX_TEMPO - MIN_TEMPO);
  return 1;
}

int createSongPattern(aleaSong* song)
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

  return 1;
}

int initSongPhrases(aleaSong* song)
{
  for (int i=0; i<PHRASE_COUNT; i++) {
    PhraseID _phraseID = song->structure[i];
    aleaPhrase* phrase = &(song->m_phrases[i]);
    phrase->octave = &(song->octave);
    memcpy(phrase->m_progression, song->phrase_dict[_phraseID], 
        sizeof(song->phrase_dict[_phraseID]));
    initPhraseMeasures(phrase);
  }
  return 1;
}

int initPhraseMeasures(aleaPhrase* phrase)
{
  for (int i=0; i<MEASURE_COUNT; i++) {
    aleaMeasure* measure = &(phrase->m_measures[i]);
    if (!chordFromProgression(&(measure->m_chord), 
          phrase->octave, 
          phrase->m_progression[i])) {
      fprintf(stderr, "Couldn't initialize measures\n");
      return 0;
    }
    if (!populateMeasure(measure)) {
      fprintf(stderr, "Couldn't populate measures\n");
      return 0;
    }
  }
  return 1;
}

int populateMeasure(aleaMeasure* measure)
{
  int irand = 0;
  chord* chd = &(measure->m_chord);
  for (int i=0; i<8; i++) {
    aleaNote* nt = &(measure->m_notes[i]);
    nt->len = 0.125;
    nt->count = 1;
    irand = rand() % 10;
    if (irand < 8) {
      irand = rand() % chd->num_chord_notes;
      nt->freq[0] = chd->chord_notes[irand];
    } else {
      irand = rand() % chd->num_nonchord_notes;
      nt->freq[0] = chd->nonchord_notes[irand];
    }
  }
  return 1;
}

int chordFromProgression(chord* cchord, float (*octave)[13], int m_chord[3])
{
  int found = 0;
  cchord->num_chord_notes = 0;
  cchord->num_nonchord_notes = 0;
  for (int i=1; i<13; i++) {
    found = 0;
    for (int j=0; j<3; j++) {
      if (i == m_chord[j]) {
        cchord->chord_notes[cchord->num_chord_notes] = (*octave)[i];
        cchord->num_chord_notes++;
        found = 1;
        break;
      }
    }

    if (found == 1) { continue; }

    for (int j=0; j<7; j++) {
      if (i == MAJOR_SCALE_INDICES[j]) {
        cchord->nonchord_notes[cchord->num_nonchord_notes] = (*octave)[i];
        cchord->num_nonchord_notes++;
        break;
      }
    }
  }
  return 1;
}

char fmtPhraseID(PhraseID ptype) {
  switch (ptype) {
    case PhraseA:
      return 'A';
    case PhraseB:
      return 'B';
    case PhraseC:
      return 'C';
    case PhraseD:
      return 'D';
    default:
      return '\n';
  }
}

void printNote(aleaNote* note)
{
  printf("\t\t\tPRINTING NOTE\n");
  for (int i=0; i<note->count; i++) {
    printf("\t\t\t\tNOTE %d: %.4f Hz\n", i, note->freq[i]);
    printf("\t\t\t\tNOTE %d: %.4f note length\n", i, note->len);
  }
}

void printMeasure(aleaMeasure* measure)
{
  chord* chd = &(measure->m_chord);
  printf("\t\tPRINTING MEASURE\n");
  for (int i=0; i<chd->num_chord_notes; i++) {
    printf("\t\t\tCHORD NOTE %d: %.4f Hz\n", i, chd->chord_notes[i]);
  }
  for (int i=0; i<chd->num_nonchord_notes; i++) {
    printf("\t\t\tNONCHORD NOTE %d: %.4f Hz\n", i, chd->nonchord_notes[i]);
  }
  for (int i=0; i<8; i++) {
    printNote(&(measure->m_notes[i]));
  }
}

void printPhrase(aleaPhrase* phrase)
{
  printf("\tPRINTING PHRASE\n");
  for (int i=0; i<PROGRESSION_LENGTH; i++) {
    printf("\t\tPROGRESSION %d: %d-%d-%d\n", i,
        phrase->m_progression[i][0],
        phrase->m_progression[i][1],
        phrase->m_progression[i][2]);
  }
  for (int i=0; i<MEASURE_COUNT; i++) {
    printMeasure(&(phrase->m_measures[i]));
  }
}

void printSong(aleaSong* song)
{
  printf("PRINTING SONG\n");
  printf("\tOCTAVE: %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
      song->octave[1],song->octave[2],song->octave[3],song->octave[4],
      song->octave[5],song->octave[6],song->octave[7],song->octave[8],
      song->octave[9],song->octave[10],song->octave[11],song->octave[12]);
  printf("\tTEMPO: %d bpm\n", song->tempo_bpm);
  printf("\tSTRUCTURE: %c-%c-%c-%c-%c-%c\n", 
      fmtPhraseID(song->structure[0]), fmtPhraseID(song->structure[1]),
      fmtPhraseID(song->structure[2]), fmtPhraseID(song->structure[3]),
      fmtPhraseID(song->structure[4]), fmtPhraseID(song->structure[5]));
  for (int i=0; i<PHRASE_COUNT; i++) {
    printPhrase(&(song->m_phrases[i]));
  }
}
