#include "MIDITranslate.hh"
#include "NoteSmooth.hh"
#include <Audio.h>
#include <Wire.h>

#define VELOCITY 127
#define CHANNEL 1

//const int Mic = AUDIO_INPUT_LINEIN;
const int Mic = AUDIO_INPUT_MIC;
const int BUTTON_PIN = 14;
const int LED_PIN = 22;

AudioInputI2S             audioInput;         // audio shield: mic or line-in
AudioAnalyzeNoteFrequency notefreq;
AudioAnalyzePeak           peak;
AudioOutputI2S             audioOutput;        // audio shield: headphones & line-out

AudioConnection patchCord2(audioInput, 0, notefreq, 0);
AudioConnection patchCord3(audioInput, 0, peak, 0);
AudioConnection patchCord4(audioInput, 0, audioOutput, 0);
AudioConnection patchCord5(audioInput, 0, audioOutput, 1);

AudioControlSGTL5000 audioShield;


int note;
int lastNote;
bool noteSmoothingOn;
float freq;
float prob;
float magnitude;

void setup() {
  AudioMemory(60);
  Serial.begin(9600);

  audioShield.enable();
  audioShield.inputSelect(Mic);
  audioShield.micGain(50); 

  notefreq.begin(0.3);

  note = -1;
  lastNote = -1;
  magnitude = 0;

  noteSmoothingOn = false;
}

void loop() {
  if (peak.available()) {
      magnitude = peak.read();
  }

  if (magnitude < 0.1) {
    stopMidi(lastNote);
  }
  
  if (notefreq.available())
  {

    freq = notefreq.read();
    prob = notefreq.probability();

    note = getMidiVal(freq);

    if (noteSmoothingOn) {
        addNote(note);
        note = smoothedNote(lastNote);
    }
    
    if (prob > 0.90)
    {
      // freq print to serial for testing
      Serial.printf("F[0] = %.3f\t | Prob = %.3f\%\t |midiVal = %d\t\n", freq, prob, note);

      if (note != lastNote) {

        if (lastNote != -1) {
          stopMidi(lastNote);
        }

        if (note != -1) {
          sendMidi(note);
          lastNote = note;
        }
        
      } 
    } else {
      if (lastNote != -1) {
        stopMidi(lastNote);
      }
      lastNote = -1;
    }
  }

}


/*****************************************************************************\

\*****************************************************************************/

void sendMidi(int note) {
  usbMIDI.sendNoteOn(note, VELOCITY, CHANNEL);
  while (usbMIDI.read());
}

void changeMidi(int note, int lastNote) {
  stopMidi(lastNote);
  sendMidi(note);
}

void stopMidi(int note) {
  usbMIDI.sendNoteOff(note, 0, CHANNEL);
  //turnNotesOff();
  while (usbMIDI.read());
}

void turnNotesOff() {
  for (int i = 0; i < rangeSize; i++) {
    usbMIDI.sendNoteOff(minMidi + i, 0, CHANNEL);
  }
}

/*****************************************************************************\

\*****************************************************************************/
