#include "window.hpp"

#include <cmath>
#include <numbers>

namespace dsp {

namespace {

class HannWindow : public Window {
public:
  WindowKind getKind() const override { return WindowKind::Hann; }

  void calculateCoefficients(int width) override {
    const float a0 = 0.5f;
    const float a1 = 0.5f;

    for (int i = 0; i < width; ++i) {
      const float k = 2 * std::numbers::pi * i;

      float real = a0 - a1 * std::cos(k / (width - 1));
      coefficients[i] = std::complex(real, 0.0f);
    }
  }
};

class HammingWindow : public Window {
public:
  WindowKind getKind() const override { return WindowKind::Hamming; }

  void calculateCoefficients(int width) override {
    const float a0 = 0.5f;
    const float a1 = 0.5f;

    for (int i = 0; i < width; ++i) {
      const float k = 2 * std::numbers::pi * i;

      float real = a0 - a1 * std::cos(k / (width - 1));
      coefficients[i] = std::complex(real, 0.0f);
    }
  }
};

class NuttallWindow : public Window {
public:
  WindowKind getKind() const override { return WindowKind::Nuttall; }

  void calculateCoefficients(int width) override {
    const float a0 = 0.355768f;
    const float a1 = 0.487396f;
    const float a2 = 0.144232f;
    const float a3 = 0.012604f;

    for (int i = 0; i < width; ++i) {
      const float k = 2 * std::numbers::pi * i;

      float real = a0 - a1 * std::cos(k / width) +
                   a2 * std::cos(2 * k / width) - a3 * std::cos(3 * k / width);
      coefficients[i] = std::complex(real, 0.0f);
    }
  }
};

} // namespace

Window *Window::createWindow(WindowKind kind) {
  switch (kind) {
  case WindowKind::Hann:
    return new HannWindow();
  case WindowKind::Hamming:
    return new HammingWindow();
  case WindowKind::Nuttall:
    return new NuttallWindow();
  default:
    return nullptr;
  }
}

} // namespace dsp
