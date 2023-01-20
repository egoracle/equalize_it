#include "spectrogram.hpp"

#include <cmath>
#include <numbers>

shared::core::audio::Spectrogram::Spectrogram(int fftSize, Window &window)
    : fftSize(fftSize), window(window), phasors(fftSize) {}

void shared::core::audio::Spectrogram::update(float signal[]) {
  for (int i = 0; i < fftSize; i++) {
    phasors[i] = std::complex<double>(window(i) * signal[i], 0);
  }

  fft(phasors);
}

void shared::core::audio::Spectrogram::fft(
    std::valarray<std::complex<double>> &fftData) {
  const int n = fftData.size();
  if (n == 1) {
    return;
  }

  std::valarray<std::complex<double>> odd = fftData[std::slice(1, n / 2, 2)];
  fft(odd);

  std::valarray<std::complex<double>> even = fftData[std::slice(0, n / 2, 2)];
  fft(even);

  for (int k = 0; k < n / 2; k++) {
    std::complex<double> t =
        std::polar(1.0, -2 * std::numbers::pi * k / n) * odd[k];
    fftData[k] = even[k] + t;
    fftData[k + n / 2] = even[k] - t;
  }
}

void shared::core::audio::Spectrogram::unloadAmplitudes(float amplitudes[]) {
  for (int i = 0; i < fftSize; i++) {
    amplitudes[i] = std::abs(phasors[i]) / fftSize;
  }
}