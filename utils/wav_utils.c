#include <stdlib.h>
#include "portaudio.h"
#include "wav_utils.h"

void initWavContainer(wavContainer* wav, aleaSong* song)
{
  float song_length_s;
  int beat_count;
  wav->song = song;
  wav->snd_info.samplerate = SAMPLE_RATE;
  wav->snd_info.channels = 1;
  wav->snd_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

  beat_count = 16 * PHRASE_COUNT;
  song_length_s = beat_count / (song->tempo_bpm / 60);
  song_length_s = ceil(song_length_s); // Round up to the next sec to have some spare buffers
  wav->buf_count = SAMPLE_RATE * song_length_s;
  wav->wav_data = malloc(wav->buf_count * sizeof(s16));
  wav->buf_whole_note = SAMPLE_RATE * (4 / (song->tempo_bpm / 60));

  wav->iter = 0;
}

/// Write a single aleaNote object to the WAV data
/// Returns the amount of samples that were written
u32 write_note(wavContainer* wav, aleaNote* note)
{
  float freq_0 = 0.0f; 
  float freq_1 = 0.0f;
  double base_amplitude = QUARTER_AMPLITUDE;
  double amplitude_f_0 = 0.0f;
  double amplitude_f_1 = 0.0f;
  double amplitude_factor;
  double t = 0;
  u32 note_buf = wav->buf_whole_note * note->len;
  u32 iter_old = wav->iter;
  u32 num_attack_buf = ATTACK_ENVELOPE_MS * SAMPLE_RATE / 1000;
  u32 num_release_buf = RELEASE_ENVELOPE_MS * SAMPLE_RATE / 1000;

  freq_0 = 1 / (note->freq[0]);
  if (note->count > 1) {
    // if there's too many notes, high amplitude sounds terrible, so we halve the
    // amplitude to reduce the gain
    base_amplitude = EIGHTH_AMPLITUDE; 
    freq_1 = 1 / (note->freq[1]);
  }

  for (int i=0; i<note_buf; i++) {
    if (i < num_attack_buf) {
      amplitude_factor = (base_amplitude * i) / num_attack_buf;
    } else if ((note_buf - i) < num_release_buf) {
      amplitude_factor = (base_amplitude * (note_buf-i)) / num_release_buf;
    } else {
      amplitude_factor = base_amplitude;
    }
    t = (double)wav->iter / SAMPLE_RATE;
    amplitude_f_0 = 2 * (t/freq_0 - floor(1/2 + t/freq_0));
    if (freq_1 != 0.0f) {
      amplitude_f_1 = 2 * (t/freq_1 - floor(1/2 + t/freq_1));
    }
    wav->wav_data[wav->iter++] += amplitude_factor * (amplitude_f_0 + amplitude_f_1);
  }

  return (wav->iter - iter_old);
}

void write_song(wavContainer* wav)
{
  wav->iter = 0;
  aleaSong* _song = wav->song;
  for (int phrase_idx=0; phrase_idx<PHRASE_COUNT; phrase_idx++) {
    ///
    aleaPhrase* _phrase = &(_song->m_phrases[_song->structure[phrase_idx]]);
    for (int meas_idx=0; meas_idx<MEASURE_COUNT; meas_idx++) {
      ///
      aleaMeasure* _measure = &(_phrase->m_measures[meas_idx]);
      if ((wav->flags & FLAG_BASS) == FLAG_BASS) {
        aleaNote* _bassnote = &(_measure->bassline);
        u32 _it = write_note(wav, _bassnote);
        wav->iter -= _it; /// "Rewinding" the iterator back to the start of the measure
      }
      for (int note_idx=0; note_idx<8; note_idx++) {
        aleaNote* _note = &(_measure->m_notes[note_idx]);
        write_note(wav, _note);      
      }
    }
  }
}

void write_song_to_file(wavContainer* wav, const char *filename)
{
  SNDFILE* wav_file = sf_open(filename, SFM_WRITE, &wav->snd_info);
  sf_writef_short(wav_file, wav->wav_data, wav->buf_count);
  sf_close(wav_file);
}

int wav_playback(wavContainer* wav)
{
  PaError err = Pa_Initialize();
  if (err != paNoError) goto error;

  PaStreamParameters input_params = {
    .channelCount = 1, 
    .sampleFormat = paInt16,
  };
  PaStreamParameters output_params = {
    .device = Pa_GetDefaultOutputDevice(),
    .channelCount = 1,
    .sampleFormat = paInt16,
  };
  output_params.suggestedLatency = Pa_GetDeviceInfo(output_params.device)->defaultHighOutputLatency;

  PaStream *stream;
  err = Pa_OpenStream(
      &stream,
      &input_params, &output_params,
      SAMPLE_RATE, SAMPLE_RATE, // this makes each buffer a full second long
      paClipOff,
      NULL, NULL);
  if (err != paNoError) goto error;

  err = Pa_StartStream(stream);
  if (err != paNoError) goto error;
  printf("Starting stream\n");
  fflush(stdout);

  // The whole buffer will be really large, so we write one second at a time
  u32 play_iter = 0;
  while (play_iter < (wav->buf_count - SAMPLE_RATE)) {
    Pa_WriteStream(stream, &wav->wav_data[play_iter], SAMPLE_RATE);
    play_iter += SAMPLE_RATE;
  }
  // flush the remaining buffers
  Pa_WriteStream(stream, &wav->wav_data[play_iter], (wav->buf_count % SAMPLE_RATE));

  err = Pa_StopStream(stream);
  if (err != paNoError) goto error;
  err = Pa_CloseStream(stream);
  if (err != paNoError) goto error;

  Pa_Terminate();
  return 0;

error:
  printf("Error occured with PortAudio: %s\n", Pa_GetErrorText(err));
  Pa_Terminate();
  return 1;
}
