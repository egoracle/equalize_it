#pragma once

#include <complex>
#include <valarray>

namespace dsp {

enum class WindowKind { Hann, Hamming, Nuttall };

class Window {
public:
  void updateCoefficients(int width) {
    coefficients.resize(width);
    calculateCoefficients(width);
  }

  const std::valarray<std::complex<float>> &getCoefficients() const noexcept {
    return coefficients;
  }

  virtual WindowKind getKind() const = 0;

  static Window *createWindow(WindowKind);

protected:
  virtual void calculateCoefficients(int) = 0;

  std::valarray<std::complex<float>> coefficients;
};

} // namespace dsp
