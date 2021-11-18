#ifndef FFT_H
#define FFT_H

#include <Arduino.h>
  
  extern float f_peaks[5];

  float FFT(int in[], int N, float Frequency);

  float sine(int i);

  float cosine(int i);

#endif