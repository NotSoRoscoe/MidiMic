#include "MIDITranslate.hh"

float NOTE_FREQS[38] = {103.826, 110.000, 116.541, 123.471, 130.813, // G#2, A2,  A#2, B2, C3
                        138.591, 146.832, 155.564, 164.814, 174.614, // C#3, D3,  D#3, E3, F3
                        184.997, 195.998, 207.652, 220.000, 233.082, // F#3, G3,  G#3, A3, A#3
                        246.942, 261.626, 277.182, 293.665, 311.127, // B3,  C4,  C#4, D4, D#4
                        329.628, 349.228, 369.994, 391.995, 415.305, // E4,  F4,  F#4, G4, G#4
                        440.000, 466.164, 493.883, 523.251, 554.365, // A4,  A#4, B4,  C5, C#5
                        587.330, 622.254, 659.255, 698.457, 739.988, // D5,  D#5, E5,  F5, F#5 
                        783.991, 830.609, 880.000};                  // G5,  G#5, A5

int MIDI_VALS[38] =  {44, 45, 46, 47, 48,  // G#2, A2,  A#2, B2, C3
                      49, 50, 51, 52, 53,  // C#3, D3,  D#3, E3, F3	
                      54, 55, 56, 57, 58,  // F#3, G3,  G#3, A3, A#3
                      59, 60, 61, 62, 63,  // B3,  C4,  C#4, D4, D#4
                      64, 65, 66, 67, 68,  // E4,  F4,  F#4, G4, G#4
					            69, 70, 71, 72, 73,  // A4,  A#4, B4,  C5, C#5
					            74, 75, 76, 77, 78,  // D5,  D#5, E5,  F5, F#5 
					            79, 80, 81};				 // G5,  G#5, A5

int rangeSize = 37; 
int minMidi = MIDI_VALS[0];

int getMidiVal(float freq) {
  
	int midpoint;

	for (int i = 0; i < rangeSize; i++) {
		midpoint = (NOTE_FREQS[i] + NOTE_FREQS[i+1]) / 2;

		// note range not supported
		if ((freq < NOTE_FREQS[0]) || (freq >= NOTE_FREQS[rangeSize])) {
			return -1;
		}

		if( freq < midpoint) {
			return MIDI_VALS[i];
		} 
	}

  return -1;
}
