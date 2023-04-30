#include "fft.hpp"

#include <cmath>
#include <numbers>

namespace dsp {

void fft(ComplexArray &phasors) {
  const int n = phasors.size();
  if (n == 1) {
    return;
  }

  ComplexArray odd = phasors[std::slice(1, n / 2, 2)];
  fft(odd);

  ComplexArray even = phasors[std::slice(0, n / 2, 2)];
  fft(even);

  for (int k = 0; k < n / 2; ++k) {
    const std::complex<float> t =
        std::polar<float>(1.0, -2 * std::numbers::pi * k / n) * odd[k];

    phasors[k] = even[k] + t;
    phasors[k + n / 2] = even[k] - t;
  }
}

std::function<float(int)> binToFrequencyMapping(int fftSize, float sampleRate) {
  return [=](int index) { return float(index) * sampleRate / float(fftSize); };
}

} // namespace dsp
