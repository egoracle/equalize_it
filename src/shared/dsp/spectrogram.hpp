#pragma once

#include "window.hpp"

#include <complex>
#include <valarray>

namespace dsp {

class Spectrogram {
public:
  Spectrogram(int, dsp::Window &);

  void update(float[]);
  void unloadAmplitudes(float[]);

private:
  void fft(std::valarray<std::complex<double>> &);

private:
  int fftSize;
  dsp::Window &window;
  std::valarray<std::complex<double>> phasors;
};

}; // namespace dsp
