#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef enum {
  PhraseA,
  PhraseB,
  PhraseC,
  PhraseD
} PhraseID;
typedef uint8_t u8;
typedef int16_t s16;

struct scale {
  float notes[8];
}

struct chord {
  struct *scale m_scale;
  s16 chord_notes[3];
}

struct songPhrase {
  PhraseID phrase_id;
  u8 progression[4]
}

struct aleaNote {
  s16* freq; // frequency of note in hz. dynamically-allocated for multiple notes
  float len; // length of note--1.0 is a whole note, 0.125 an eight note
  u8 count;  // length of *freq buffer
}

struct aleaMeasure {

}

struct aleaLine {

}

struct aleaSong {
  struct songPhrase phrase_dict[4];

}
