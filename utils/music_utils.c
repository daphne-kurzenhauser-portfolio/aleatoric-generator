#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "music_tbls.h"
#include "music_utils.h"

const float hz_lut[NUM_NOTES] = {  
0.0,  0.0,   0.0,  0.0,   0.0,  0.0,  0.0,   0.0,  0.0,   0.0,  0.0,   0.0,
C0_Hz,Cs0_Hz,D0_Hz,Ds0_Hz,E0_Hz,F0_Hz,Fs0_Hz,G0_Hz,Gs0_Hz,A0_Hz,As0_Hz,B0_Hz,
C1_Hz,Cs1_Hz,D1_Hz,Ds1_Hz,E1_Hz,F1_Hz,Fs1_Hz,G1_Hz,Gs1_Hz,A1_Hz,As1_Hz,B1_Hz,
C2_Hz,Cs2_Hz,D2_Hz,Ds2_Hz,E2_Hz,F2_Hz,Fs2_Hz,G2_Hz,Gs2_Hz,A2_Hz,As2_Hz,B2_Hz,
C3_Hz,Cs3_Hz,D3_Hz,Ds3_Hz,E3_Hz,F3_Hz,Fs3_Hz,G3_Hz,Gs3_Hz,A3_Hz,As3_Hz,B3_Hz,
C4_Hz,Cs4_Hz,D4_Hz,Ds4_Hz,E4_Hz,F4_Hz,Fs4_Hz,G4_Hz,Gs4_Hz,A4_Hz,As4_Hz,B4_Hz,
C5_Hz,Cs5_Hz,D5_Hz,Ds5_Hz,E5_Hz,F5_Hz,Fs5_Hz,G5_Hz,Gs5_Hz,A5_Hz,As5_Hz,B5_Hz,
C6_Hz,Cs6_Hz,D6_Hz,Ds6_Hz,E6_Hz,F6_Hz,Fs6_Hz,G6_Hz,Gs6_Hz,A6_Hz,As6_Hz,B6_Hz,
C7_Hz,Cs7_Hz,D7_Hz,Ds7_Hz,E7_Hz,F7_Hz,Fs7_Hz,G7_Hz,Gs7_Hz,A7_Hz,As7_Hz,B7_Hz,
C8_Hz,Cs8_Hz,D8_Hz,Ds8_Hz,E8_Hz,F8_Hz,Fs8_Hz,G8_Hz,Gs8_Hz,A8_Hz,As8_Hz,B8_Hz,
C9_Hz
};

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

const int MAJOR_SCALE_INDICES[7] = { 1, 3, 5, 6, 8, 10, 12 };

const PhraseID songStructures[][PHRASE_COUNT] = {
  {PhraseA, PhraseA, PhraseB, PhraseB, PhraseC, PhraseC},
  {PhraseA, PhraseB, PhraseA, PhraseB, PhraseC, PhraseD},
  {PhraseA, PhraseB, PhraseC, PhraseD, PhraseD, PhraseD}
};

void setSongKey(aleaSong* song)
{
  srand(time(NULL));
  song->key_note = ((rand() % (MAX_STARTING_KEY_MIDI-MIN_STARTING_KEY_MIDI)) + MIN_STARTING_KEY_MIDI);
}

void setSongTempo(aleaSong* song)
{
  srand(time(NULL));
  song->tempo_bpm = (float)((rand() % (MAX_TEMPO-MIN_TEMPO)) + MIN_TEMPO);
}

void createSongPattern(aleaSong* song)
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
}

int initSongPhrases(aleaSong* song)
{
  for (int i=0; i<4; i++) {
    PhraseID _phraseID = i;
    aleaPhrase* phrase = &(song->m_phrases[i]);
    memcpy(phrase->m_progression, song->phrase_dict[_phraseID], 
        sizeof(song->phrase_dict[_phraseID]));
    phrase->key_note = song->key_note;
    phrase->flags = song->flags;
    initPhraseMeasures(phrase);
  }
  return 1;
}

int initPhraseMeasures(aleaPhrase* phrase)
{
  for (int i=0; i<MEASURE_COUNT; i++) {
    aleaMeasure* measure = &(phrase->m_measures[i]);
    measure->m_chord.key_root = phrase->key_note;
    if (!chordFromProgression(&(measure->m_chord), phrase->m_progression[i])) {
      fprintf(stderr, "Couldn't initialize measures\n");
      return 0;
    }
    populateMeasure(measure, phrase->flags);
  }
  return 1;
}

void populateMeasure(aleaMeasure* measure, u8 flags)
{
  int irand = 0;
  chord* chd = &(measure->m_chord);
  for (int i=0; i<8; i++) {
    aleaNote* nt = &(measure->m_notes[i]);
    Note melody_note;
    nt->len = 0.125;
    nt->count = 1;
    irand = rand() % 10;
    if (irand < 8) {
      irand = rand() % chd->num_chord_notes;
      melody_note = chd->chord_notes[irand];
      nt->freq[0] = hz_lut[melody_note];
    } else {
      irand = rand() % chd->num_nonchord_notes;
      melody_note = chd->nonchord_notes[irand];
      nt->freq[0] = hz_lut[melody_note];
    }
    if ((flags & FLAG_HARMONY) == (FLAG_HARMONY)) {
      int found = 0;
      Note harmony_note;
      nt->count++;
      for (int j=(chd->num_chord_notes-1); j>=0; j--) {
        if (chd->chord_notes[j] < melody_note) {
          harmony_note = chd->chord_notes[j];
          nt->freq[1] = hz_lut[harmony_note];
          found = 1;
          break;
        }
      }
      if (found == 1) { continue; }

      /// if there wasn't a harmony note lower than the melody note, then we need
      /// to loop back around to the highest chord note and drop an octave down
      harmony_note = chd->chord_notes[chd->num_chord_notes-1] - 12;
      nt->freq[1] = hz_lut[harmony_note];
    }
  }
  if ((flags & FLAG_BASS) == (FLAG_BASS)) {
    aleaNote* bass_nt = &(measure->bassline[0]);
    Note bass_note = chd->chord_notes[0] - 24;
    bass_nt->freq[0] = hz_lut[bass_note];
    bass_nt->len = 1.0;
    bass_nt->count = 1;
  }
  return;
}

int chordFromProgression(chord* cchord, int m_chord[3])
{
  int found = 0;
  cchord->num_chord_notes = 0;
  cchord->num_nonchord_notes = 0;
  for (int i=1; i<13; i++) {
    found = 0;
    for (int j=0; j<3; j++) {
      if (i == m_chord[j]) {
        cchord->chord_notes[cchord->num_chord_notes] = cchord->key_root+(i-1);
        cchord->num_chord_notes++;
        found = 1;
        break;
      }
    }

    if (found == 1) { continue; }

    for (int j=0; j<7; j++) {
      if (i == MAJOR_SCALE_INDICES[j]) {
        cchord->nonchord_notes[cchord->num_nonchord_notes] = cchord->key_root+(i-1);
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
  for (int i=0; i<note->count; i++) {
    printf("\t\t\t\tNOTE %d: %.4f Hz, length = %.4f\n", i, note->freq[i], note->len);
  }
}

void printMeasure(aleaMeasure* measure)
{
  chord* chd = &(measure->m_chord);
  printf("\t\tPRINTING MEASURE\n");
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
  printf("\tTEMPO: %.1f bpm\n", song->tempo_bpm);
  printf("\tSTRUCTURE: %c-%c-%c-%c-%c-%c\n", 
      fmtPhraseID(song->structure[0]), fmtPhraseID(song->structure[1]),
      fmtPhraseID(song->structure[2]), fmtPhraseID(song->structure[3]),
      fmtPhraseID(song->structure[4]), fmtPhraseID(song->structure[5]));
  for (int i=0; i<PHRASE_COUNT; i++) {
    printPhrase(&(song->m_phrases[song->structure[i]]));
  }
}
