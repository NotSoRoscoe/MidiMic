#include "MIDITranslate.hh"
//#include "FFT.hh"
#include <arduinoFFT.h>

#define SAMPLE_SIZE 1024      // Amount of samples to take before running pitch detection
#define SAMPLE_FREQ 44100     // Sample Frequency for arduino fft
#define THRESHHOLD 50         // Amplitude of a single sample to reach before running pitch detection
#define ANALOG_INPUT 22       // Analog pin used to read samples
#define OFFSET 512            // AnalogRead converts to values between 0-1024. Centers waveform around half (3.3V/2) 
#define MICRO 0.000001        // For converting values into microseconds
#define SAMPLE_PERIOD 1 / (SAMPLE_FREQ * MICRO) //Used to ensure sample period when sampling

// Defintions for old fft function
// May be removed 
#define SAMPLE_SPEED 124       // Amount of time between samples. Value in microseconds. Min Value is BASE_SAMPLE_SPEED
#define BASE_SAMPLE_SPEED 17  // Tested value of how long a single sample takes to read with no delay. Value is in microseconds
#define TEST_SAMPLE_FREQ 1 / (SAMPLE_SPEED * MICRO) // Testing an FFT function 
#define NUMPEAKS 2          // Number of frequancy peaks to compare 1 min, 5 max.

//MIDI STUFF
#define CHANNEL 1
#define VELOCITY 127
#define MIN_MIDI_NOTE 44
int note;
int lastNote;

double vReal[SAMPLE_SIZE];
double vImag[SAMPLE_SIZE];
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLE_SIZE, SAMPLE_FREQ);

/*****************************************************************************\

  \*****************************************************************************/


void setup() {
  Serial.begin(9600);
  note = -1;
  lastNote = 0;
}

void loop() {

  bool threshReached;
  double freq;
  float sum;
  float avg;

  // Reset every sample
  sum = 0;
  avg = 0;
  threshReached = false;


  long start;
  long end;
  long sampleStart;

  start = micros();

  // Reading Samples from analog input

  for (int i = 0; i < SAMPLE_SIZE; i++)
  {
    sampleStart = micros();
    vReal[i] = analogRead(ANALOG_INPUT) - OFFSET;
    vImag[i] = 0;
    sum = sum + abs(vReal[i]);
    while ((micros() - sampleStart) < SAMPLE_PERIOD); // keeping sampling frequency consistent
  }
  
  end = micros();

  // Determine Threshhold
  avg = sum / SAMPLE_SIZE;
  if (avg > THRESHHOLD) {
    threshReached = true;
  }



  // Perform FFT and Play Note

  if (threshReached) {

    // Run Pitch Detection
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();
    freq = FFT.MajorPeak(vReal, SAMPLE_SIZE, SAMPLE_FREQ);

    //old fft. May Delete
    //FFT(samples, SAMPLE_SIZE, TEST_SAMPLE_FREQ);
    //freq = minPeak(f_peaks);


    note = getMidiVal((float) freq);
    
    // freq print to serial for testing
    Serial.printf("F[0] = %.3f\tSmpl Time = %dus\tavg = %.2f\tmidiVal = %d\n", freq, end - start, avg, note);

    

    if (note != lastNote) {
      changeMidi(note, lastNote);
    } else if (note == -1) {
      sendMidi(note);
    }

    lastNote = note;

  } else {

    if (note != -1) {
      stopMidi(note);
      note = -1;
    }

  }

}

/*****************************************************************************\

  \*****************************************************************************/

//Used with old fft. May Delete
float minPeak(float* arr) {
  float temp = arr[0];
  for (int i = 1; i < NUMPEAKS; i++) {
    if (temp > arr[i])
      temp = arr[i];
  }
  return temp;
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
    usbMIDI.sendNoteOff(MIN_MIDI_NOTE + i, 0, CHANNEL);
  }
}

/*****************************************************************************\

  \*****************************************************************************/
