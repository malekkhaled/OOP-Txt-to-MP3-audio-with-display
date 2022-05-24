#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "song.h"
using namespace std;

Note :: Note (string str)
{
    istringstream tempStream;
    tempStream.str(str);
    char tempChar, tempChar2;
    string str2;

    tempStream >> tempChar;
    tempStream >> tempChar2;

    if(!isdigit(tempChar2))
    {
        str2 += tempChar;
        str2 += tempChar2;
        octave = tempStream.get() - '0';
    }
    else
    {
        str2 += tempChar;
        octave = tempChar2 - '0';
    }
            
    if(str2 == "C")
        pitch = C;
    else if (str2 == "Db" || str2 == "C#")
        pitch = Db;
    else if (str2 == "D")
        pitch = D;
    else if (str2 == "Eb" || str2 == "D#")
        pitch = Eb;
    else if (str2 == "E")
        pitch = E;
    else if (str2 == "F")
        pitch = F;
    else if (str2 == "Gb" || str2 == "F#")
        pitch = Gb;
    else if (str2 == "G")
        pitch = G;
    else if (str2 == "Ab" || str2 == "G#")
        pitch = Ab;
    else if (str2 == "A")
        pitch = A;
    else if (str2 == "Bb" || str2 == "A#")
        pitch = Bb;
    else if (str2 == "B")
        pitch = B;
    else if (str2 == "REST" || str2 == "R")
        pitch = REST;

    str2.clear();
    tempStream >> tempChar;
    if(tempStream >> tempChar2)
    {
        str2 += tempChar;
        str2 += tempChar2;
    }
    else
        str2 += tempChar;
    
    if (str2 == "q")
        duration = 1;
    else if (str2 == "q.")
        duration = 1 * 1.5;
    else if (str2 == "h")
        duration = 2;
    else if (str2 == "h.")
        duration = 2 * 1.5;
    else if (str2 == "w")
        duration = 4;
    else if (str2 == "w.")
        duration = 4 * 1.5;
    else if (str2 == "e")
        duration = 0.5;
    else if (str2 == "e.")
        duration = 0.5 * 1.5;
    else if (str2 == "s")
        duration = 0.25;
    else if (str2 == "s.")
        duration = 0.25 * 1.5;      
}

Voice :: Voice (const string& str, double vol, instrument_t instr)
{
    volume = vol;
    instrument = instr;
    istringstream tempStream2;
    string str2;
    tempStream2.str(str);
    
    while (tempStream2 >> str2)
    { 
        Note n(str2);
        addNote(n);
    }
}

Song :: Song (const string& file)
{
    ifstream input (file);
    string str,str2;
    input >> tempo;
    getline(input, str2);
    while (!input.eof())
    {
        getline(input, str);
        Voice v (str);
        addVoice(v);
    }
}

ostream& operator<<(ostream& out, const Note& n)
{
    switch (n.getPitch())
    {
        case A:
            out << "A";
            break;
        case B:
            out << "B";
            break;
        case C: 
            out << "C";
            break;
        case D:
            out << "D";
            break;
        case E:
            out << "E";
            break;
        case F:
            out << "F";
            break;
        case G:
            out << "G";
            break;
        case Ab:
            out << "Ab";
            break;
        case Bb:
            out << "Bb";
            break;
        case Db:
            out << "Db";
            break;
        case Eb:
            out << "Eb";
            break;
        case Gb:
            out << "Gb";
            break;
    }
    
    out << n.getOctave();
    double duration = n.getDuration();
    if (duration == 1)
        out << "q";
    else if (duration == 1.5)
        out << "q.";
    else if (duration == 2)
        out << "h";
    else if (duration == 3)
        out << "h.";
    else if (duration == 4)
        out << "w";
    else if (duration == 6)
        out << "w.";
    else if (duration == 0.5)
        out << "e";
    else if (duration == 0.75)
        out << "e.";
    else if (duration == 0.25)
        out << "s";
    else if (duration == 0.375)
        out << "s.";    

    return out;
}

ostream& operator<<(ostream& out, const Voice& v)
{
    for (int i=0; i < v.getNotes().size(); i++)
        out << v.getNotes().at(i) << " ";
    
    return out;
}

ostream& operator<<(ostream& out, const Song& s)
{
    out << s.getTempo() << endl;
    for ( int i = 0; i < s.getVoices().size(); i++)
        out << s.getVoices().at(i) << endl;

    return out;
}

