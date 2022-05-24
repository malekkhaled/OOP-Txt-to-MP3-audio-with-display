#ifndef __WAV_FILE_WRITER
#define __WAV_FILE_WRITER

#include "song.h"

#include <cstdint>
#include <string>
using namespace std;

//Basic unit of audio data
typedef int16_t sample_t;
const int BYTES_PER_SAMPLE = sizeof(sample_t);

//Important constants
const int BITS_PER_BYTE = 8;
const int SECONDS_PER_MINUTE = 60;
const double EPSILON = 1e-10;
const double FREQUENCY[] = { 4186.01, 4434.92, 4698.63, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040, 7458.62, 7902.13, 0 };
const uint16_t OCTAVE_OFFSET = 7;

//Mono vs. stereo audio
//Code to support stereo not written
enum channel_t { MONO = 1, STEREO = 2 };

//Class to write a .wav file
//Construct by passing in a file name
//Set the song data by using setSong, 
//or use setTempo() and addVoice() individually
//Once song data has been set, call writeToFile() to write the file
class WAVFileWriter
{
private:
  string filename;
  unsigned tempo;
  double offset;
  vector<sample_t> sample_data;
  vector<sample_t> convertVoiceToSamples(const Voice& v);
public:
  static const channel_t NUM_CHANNELS = MONO;
  static const int SAMPLE_RATE = 44100; //CD quality

  WAVFileWriter(string file) : filename(file) { offset = 0.0; tempo = 120; }
  void setSong(const Song& s);

  void addVoice(const Voice& v);
  void removeVoice(const Voice& v);
  void clear() { sample_data.clear(); }
  vector<sample_t>& getSampleData() { return sample_data; }

  void writeToFile() const;
};

#endif