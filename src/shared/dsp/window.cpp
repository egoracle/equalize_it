#include "window.hpp"

#include <cmath>
#include <numbers>

float dsp::HannWindow::operator()(int sampleIndex) {
  return 0.5f *
         (1 - std::cos(2 * std::numbers::pi * sampleIndex / (width - 1)));
}

float dsp::HammingWindow::operator()(int sampleIndex) {
  return 0.53836f -
         0.46164f * std::cos(2 * std::numbers::pi * sampleIndex / (width - 1));
}

float dsp::NuttallWindow::operator()(int sampleIndex) {
  float a0 = 0.355768f;
  float a1 = 0.487396f;
  float a2 = 0.144232f;
  float a3 = 0.012604f;

  float k = 2 * std::numbers::pi * sampleIndex;

  return a0 - a1 * std::cos(k / width) + a2 * std::cos(2 * k / width) -
         a3 * std::cos(3 * k / width);
}