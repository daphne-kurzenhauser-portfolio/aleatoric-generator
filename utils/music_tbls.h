#define  A3   220.0000
#define As3   233.0819
#define Bb3   233.0819
#define  B3   246.9417
#define  C4   261.6256
#define Cs4   277.1826
#define Db4   277.1826
#define  D4   293.6648
#define Ds4   311.1270
#define Eb4   311.1270
#define  E4   329.6276
#define  F4   349.2282
#define Fs4   369.9944
#define Gb4   369.9944
#define  G4   391.9954
#define Gs4   415.3047
#define Ab4   415.3047
#define  A4   440.0000
#define As4   466.1638
#define Bb4   466.1638
#define  B4   493.8833
#define  C5   523.2511
#define Cs5   554.3653
#define Db5   554.3653
#define  D5   587.3295
#define Ds5   622.2540
#define Eb5   622.2540
#define  E5   659.2551
#define  F5   698.4565
#define Fs5   739.9888
#define Gb5   739.9888
#define  G5   783.9909
#define Gs5   830.6094
#define Ab5   830.6094
#define  A5   880.0000

// the 0.0 is added because it allows us to index these octaves as 1-11 instead of 0-10
#define  A3_oct   { 0.0, A3, Bb3, B3, C4, Db4, D4, Eb4, E4, F4, Fs4, G4, Ab4 } 
#define Bb3_oct   { 0.0, Bb3, B3, C4, Db4, D4, Eb4, E4, F4, Fs4, G4, Ab4, A4 } 
#define  B3_oct   { 0.0, B3, C4, Db4, D4, Eb4, E4, F4, Fs4, G4, Ab4, A4, Bb4 } 
#define  C4_oct   { 0.0, C4, Db4, D4, Eb4, E4, F4, Fs4, G4, Ab4, A4, Bb4, B4 } 
#define Db4_oct   { 0.0, Db4, D4, Eb4, E4, F4, Fs4, G4, Ab4, A4, Bb4, B4, C5 } 
#define  D4_oct   { 0.0, D4, Eb4, E4, F4, Fs4, G4, Ab4, A4, Bb4, B4, C5, Db5 } 
#define Eb4_oct   { 0.0, Eb4, E4, F4, Fs4, G4, Ab4, A4, Bb4, B4, C5, Db5, D5 } 
#define  E4_oct   { 0.0, E4, F4, Fs4, G4, Ab4, A4, Bb4, B4, C5, Db5, D5, Eb5 } 
#define  F4_oct   { 0.0, F4, Fs4, G4, Ab4, A4, Bb4, B4, C5, Db5, D5, Eb5, E5 } 
#define Fs4_oct   { 0.0, Fs4, G4, Ab4, A4, Bb4, B4, C5, Db5, D5, Eb5, E5, F5 } 
#define  G4_oct   { 0.0, G4, Ab4, A4, Bb4, B4, C5, Db5, D5, Eb5, E5, F5, Fs5 } 
#define Ab4_oct   { 0.0, Ab4, A4, Bb4, B4, C5, Db5, D5, Eb5, E5, F5, Fs5, G5 } 
#define  A4_oct   { 0.0, A4, Bb4, B4, C5, Db5, D5, Eb5, E5, F5, Fs5, G5, Ab5 }
#define scale_maj { 1, 3, 5, 6, 8, 10, 12 }

#define chord_I   { 1, 5, 8 }  //1-3-5
#define chord_ii  { 3, 6, 10 } //2-4-6
#define chord_iii { 5, 8, 12 } //3-5-7
#define chord_iv  { 1, 6, 9 }  //4-6b-8
#define chord_IV  { 1, 6, 10 } //4-6-8
#define chord_v   { 3, 8, 11 } //5-7b-9
#define chord_V   { 3, 8, 12 } //5-7-9
#define chord_VI  { 1, 5, 10 } //6-8-10
#define chord_VII { 3, 6, 12 } //7-9-11

