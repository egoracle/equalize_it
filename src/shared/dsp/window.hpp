#pragma once

#include <complex>
#include <valarray>

namespace dsp {

std::valarray<std::complex<float>> hannWindow(int);
std::valarray<std::complex<float>> hammingWindow(int);
std::valarray<std::complex<float>> nuttallWindow(int);

} // namespace dsp
