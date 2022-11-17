#pragma once

#include "window.hpp"

#include <complex>
#include <valarray>

class Spectrogram {
public:
  Spectrogram(int fftSize, Window &window);

  void update(float signal[]);
  void unloadAmplitudes(float amplitudes[]);

private:
  void fft(std::valarray<std::complex<double>> &fftData);

private:
  int fftSize;
  Window &window;
  std::valarray<std::complex<double>> phasors;
};