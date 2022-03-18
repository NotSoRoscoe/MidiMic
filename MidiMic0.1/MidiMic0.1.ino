#include "MIDITranslate.hh"
#include <Audio.h>
#include <Wire.h>

#define VELOCITY 127
#define CHANNEL 1

//const int Mic = AUDIO_INPUT_LINEIN;
const int Mic = AUDIO_INPUT_MIC;

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
float freq;
float prob;
float magnitude;

void setup() {
  AudioMemory(60);
  Serial.begin(9600);

  audioShield.enable();
  audioShield.inputSelect(Mic);
  //audioShield.volume(0.9);
  audioShield.micGain(50); //TODO connect gain to poteniometer

  notefreq.begin(0.3);  // Ignores anything under this probability threshhold

  note = -1;
  lastNote = -1;
  magnitude = 0;

}

void loop() {

  //float magnitude = 0;
  //int note = -1;

  if (peak.available()) {
      magnitude = peak.read();
  }

    Serial.printf("peak %.3f\n", magnitude);

  if (notefreq.available())
  {

    freq = notefreq.read();
    prob = notefreq.probability();

    note = getMidiVal(freq);

    if (peak.available()) {
      magnitude = peak.read();
    }

    Serial.printf("peak %.3f\n", magnitude);
    
    if (prob > 0.9)
    {
      // freq print to serial for testing
      //Serial.printf("F[0] = %.3f\t | Prob = %.3f\%\t |midiVal = %d\t |peak = %.3f\n", freq, prob, note, magnitude);

      if (note != lastNote && lastNote != -1) {
        changeMidi(note, lastNote);

      } else if (note != -1 && lastNote == -1) {
        sendMidi(note);

      }

    } else {

      if (note != -1) {
        stopMidi(note);
        //Serial.printf("Note off = %d\n", note);
        note = -1;
      }
    }

    lastNote = note;
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
