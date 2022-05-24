#include "wavfilewriter.h"

#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

//Structs for WAVFileWriter::writeToFile
struct RIFF_Header
{
  int8_t chunkID[4];
  int32_t chunkSize;
  int8_t format[4];
};
struct WAVE_Format
{
  int8_t subChunkID[4];
  int32_t subChunkSize;
  int16_t audioFormat;
  int16_t numChannels;
  int32_t sampleRate;
  int32_t byteRate;
  int16_t blockAlign;
  int16_t bitsPerSample;
};
struct WAVE_Data
{
	int8_t subChunkID[4];
	int32_t subChunkSize;
};

//Helper function that uses the current tempo and offset to create WAV data for a Voice
vector<sample_t> WAVFileWriter::convertVoiceToSamples(const Voice& v)
{
  vector<sample_t> samples;
  const vector<Note>& notes = v.getNotes();
  const instrument_t& inst = v.getInstrument();
  double norm = 0.0;
  for (int i = 0; i < inst.size(); i++)
    norm += inst[i];
  
  for (int n = 0; n < notes.size(); n++)
  {
    double freq = FREQUENCY[notes[n].getPitch()] / (1 << (OCTAVE_OFFSET - notes[n].getOctave()));
    double delta = 2 * M_PI * freq / SAMPLE_RATE;
    int num_samples = notes[n].getDuration() * SECONDS_PER_MINUTE * SAMPLE_RATE / tempo + EPSILON;

    for (int i = 0; i < num_samples; i++)
    {
      double s = 0.0;
      for (int j = 0; j < inst.size(); j++)
        s += inst[j] * sin((j+1) * i * delta);
      samples.push_back(static_cast<sample_t>(v.getVolume() * s / norm));
    }
  }

  return samples;
}

//Sets the song to be written
//Clears any current audio data
void WAVFileWriter::setSong(const Song& s)
{
  sample_data.clear();
  tempo = s.getTempo();
  const vector<Voice>& voices = s.getVoices();
  for (int i = 0; i < voices.size(); i++)
    addVoice(voices[i]);
}

//Adds a voice to the audio data, using the current tempo
void WAVFileWriter::addVoice(const Voice& v)
{
  offset = 0.0;
  vector<sample_t> samples = convertVoiceToSamples(v);

  int min = sample_data.size();
  if (samples.size() < min)
    min = samples.size();

  for (int i = 0; i < min; i++)
    sample_data[i] += samples[i];
  for (int i = min; i < samples.size(); i++)
    sample_data.push_back(samples[i]);
}

//Removes a voice from the audio data
void WAVFileWriter::removeVoice(const Voice& v)
{
  offset = 0.0;
  vector<sample_t> samples = convertVoiceToSamples(v);

  int min = sample_data.size();
  if (samples.size() < min)
    min = samples.size();

  for (int i = 0; i < min; i++)
    sample_data[i] -= samples[i];
  for (int i = min; i < samples.size(); i++)
    sample_data.push_back(-samples[i]);
}

//Writes the current audio data to file
void WAVFileWriter::writeToFile() const
{
  struct WAVE_Data datahead;
  datahead.subChunkID[0] = 'd';
  datahead.subChunkID[1] = 'a';
  datahead.subChunkID[2] = 't';
  datahead.subChunkID[3] = 'a';
  datahead.subChunkSize = sample_data.size() * BYTES_PER_SAMPLE;

  struct WAVE_Format fmt;
  fmt.subChunkID[0] = 'f';
  fmt.subChunkID[1] = 'm';
  fmt.subChunkID[2] = 't';
  fmt.subChunkID[3] = ' ';
  fmt.subChunkSize = 16;
  fmt.audioFormat = 1;
  fmt.numChannels = NUM_CHANNELS;
  fmt.sampleRate = SAMPLE_RATE;
  fmt.byteRate = NUM_CHANNELS * BYTES_PER_SAMPLE;
  fmt.blockAlign = SAMPLE_RATE * NUM_CHANNELS;
  fmt.bitsPerSample = BYTES_PER_SAMPLE * BITS_PER_BYTE;

  struct RIFF_Header fileheader;
  fileheader.chunkID[0] = 'R';
  fileheader.chunkID[1] = 'I';
  fileheader.chunkID[2] = 'F';
  fileheader.chunkID[3] = 'F';
  fileheader.chunkSize = 36 + datahead.subChunkSize;
  fileheader.format[0] = 'W';
  fileheader.format[1] = 'A';
  fileheader.format[2] = 'V';
  fileheader.format[3] = 'E';

  ofstream out(filename, ios::binary);
  out.write((const char*) &fileheader, sizeof(struct RIFF_Header));
  out.write((const char*) &fmt, sizeof(struct WAVE_Format));
  out.write((const char*) &datahead, sizeof(struct WAVE_Data));
  out.write((const char*) sample_data.data(), sample_data.size() * BYTES_PER_SAMPLE);
  out.close();
}
