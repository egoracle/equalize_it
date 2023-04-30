#include "window.hpp"

#include <cmath>
#include <numbers>

namespace dsp {

std::valarray<std::complex<float>> hannWindow(int width) {
  std::valarray<std::complex<float>> window(width);

  const float a0 = 0.5f;
  const float a1 = 0.5f;

  for (int i = 0; i < width; ++i) {
    const float k = 2 * std::numbers::pi * i;

    float real = a0 - a1 * std::cos(k / (width - 1));
    window[i] = std::complex(real, 0.0f);
  }

  return window;
}

std::valarray<std::complex<float>> hammingWindow(int width) {
  std::valarray<std::complex<float>> window(width);

  const float a0 = 0.53836f;
  const float a1 = 0.46164f;

  for (int i = 0; i < width; ++i) {
    const float k = 2 * std::numbers::pi * i;

    float real = a0 - a1 * std::cos(k / (width - 1));
    window[i] = std::complex(real, 0.0f);
  }

  return window;
}

std::valarray<std::complex<float>> nuttallWindow(int width) {
  std::valarray<std::complex<float>> window(width);

  const float a0 = 0.355768f;
  const float a1 = 0.487396f;
  const float a2 = 0.144232f;
  const float a3 = 0.012604f;

  for (int i = 0; i < width; ++i) {
    const float k = 2 * std::numbers::pi * i;

    float real = a0 - a1 * std::cos(k / width) + a2 * std::cos(2 * k / width) -
                 a3 * std::cos(3 * k / width);
    window[i] = std::complex(real, 0.0f);
  }

  return window;
}

} // namespace dsp
