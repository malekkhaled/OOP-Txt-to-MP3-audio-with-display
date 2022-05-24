#ifndef __SONG_H
#define __SONG_H

#include <iostream>
#include <vector>
using namespace std;

//Enumerated type to represent a musical pitch
enum pitch_t { C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B, REST };
const int NUM_PITCHES = 12;

const double DEFAULT_VOLUME = 1000.0;

//Instrument data
typedef vector<double> instrument_t;
const instrument_t SINE { 1.0 };
const instrument_t ORGAN { 1.0, 1.0, 1.0 };
const instrument_t TRUMPET { 3.5, 4.5, 5.5, 4, 1.5, 1 };
const instrument_t DEFAULT_INSTRUMENT = ORGAN;

//Class representing a single note
//Constructed using a string:
//  First 1-2 characters represent pitch (A-G, possibly followed by # or b)
//  Next character is octave (0-7)
//  Last 1-2 characters are duration (q, h, w, e, s, or dotted forms)
class Note
{
private:
  pitch_t pitch;
  unsigned octave;
  double duration;
public:
  Note(string str = "R0q");
  pitch_t getPitch() const { return pitch; }
  unsigned getOctave() const { return octave; }
  double getDuration() const { return duration; }
};

//Class representing a single sequence of notes in a song
//Each Voice also has its own instrument and volume
class Voice
{
private:
  vector<Note> notes;
  instrument_t instrument;
  double volume;
public:
  Voice(const string& str = "", double vol = DEFAULT_VOLUME, instrument_t instr = DEFAULT_INSTRUMENT);
  void addNote(const Note& n) { notes.push_back(n); }
  void clear() { notes.clear(); }
  const instrument_t getInstrument() const { return instrument; }
  void setInstrument(const instrument_t& i) { instrument = i; }
  void setVolume(double v) { volume = v; }
  double getVolume() const { return volume; }
  const vector<Note>& getNotes() const { return notes; }
};

//Class representing an entire song, 
//composed of multiple voices playing notes simultaneously
class Song
{
private:
  unsigned tempo;
  vector<Voice> voices;
public:
  Song() { tempo = 120; }
  Song(const string& file);

  unsigned getTempo() const { return tempo; }
  void setTempo(unsigned t) { tempo = t; }

  void addVoice(const Voice& v) { voices.push_back(v); }
  void clear() { voices.clear(); }
  const vector<Voice>& getVoices() const { return voices; }
};

//Print function for Songs
//Should print in same format as input file
ostream& operator<<(ostream&, const Song&);

//Print function for Voices
//Should print all notes in the voice, separated by spaces
ostream& operator<<(ostream&, const Voice&);

//Print function for Notes
//Should print a note in the same format as the input file;
// e.g., C3q or Db4e.
ostream& operator<<(ostream&, const Note&);

#endif
