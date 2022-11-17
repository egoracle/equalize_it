#include "window.hpp"

#include <cmath>
#include <numbers>

float HannWindow::operator()(int sampleIndex) {
  return 0.5f *
         (1 - std::cos(2 * std::numbers::pi * sampleIndex / (width - 1)));
}

float HammingWindow::operator()(int sampleIndex) {
  return 0.53836f -
         0.46164f * std::cos(2 * std::numbers::pi * sampleIndex / (width - 1));
}