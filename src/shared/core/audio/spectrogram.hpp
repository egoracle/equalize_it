#pragma once

#include "window.hpp"

#include <complex>
#include <valarray>

namespace shared::core::audio {

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

} // namespace shared::core::audio