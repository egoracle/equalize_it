#pragma once

#include <complex>
#include <functional>
#include <valarray>

namespace dsp {

using ComplexArray = std::valarray<std::complex<float>>;

void fft(ComplexArray &);

std::function<float(int)> binToFrequencyMapping(int, float);

} // namespace dsp
