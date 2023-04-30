#pragma once

#include <complex>
#include <valarray>

namespace math {

std::valarray<std::complex<float>>
octaveSmoothing(int, std::valarray<std::complex<float>> &);

} // namespace math
