#include "MIDITranslate.hh"


float NOTE_FREQS[26] = {103.214, 111.153, 119.093, 127.033, 134.972,
            142.912, 150.851, 158.791, 166.73,  174.67,
            182.609, 198.488, 206.428, 222.307, 230.246,
            246.126, 262.005, 277.884, 293.763, 309.642,
            333.46,  349.339, 373.158, 396.977, 412.856,
            444.614 };

int MIDI_VALS[26] = { 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
            55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
            66, 67, 68, 69};

int rangeSize = 26;

int getMidiVal(float freq) {
	for (int i = 0; i < rangeSize; i++) {
		if( freq > NOTE_FREQS[i]) {
		
		} else {
			return MIDI_VALS[i];
		}
	}
}
