#include "mapping.hpp"

#include <cassert>
#include <cmath>

namespace math {

std::function<float(float)> segmentMapping(float fromMin, float fromMax,
                                           float toMin, float toMax) {
  return [=](float x) {
    x -= fromMin;             // [0; fromMax - fromMin]
    x /= (fromMax - fromMin); // [0; 1]
    x -= 0.5f;                // [-0.5; 0.5]
    x *= 2;                   // [-1; 1]
    x *= (toMax - toMin);     // [toMin - toMax; toMax - toMin]
    x += (toMax + toMin);     // [2 * toMin; 2 * toMax]
    x /= 2;                   // [toMin; toMax]

    return x;
  };
}

std::function<float(float)> logMapping(float fromMin, float fromMax,
                                       float toMin, float toMax) {
  assert(fromMin > 0);
  assert(fromMax > 0);

  auto rawMapping =
      segmentMapping(std::log10(fromMin), std::log10(fromMax), toMin, toMax);

  return [=](float x) { return rawMapping(std::log10(x)); };
}

std::function<float(float)> invLogMapping(float fromMin, float fromMax,
                                          float toMin, float toMax) {
  auto rawMapping =
      segmentMapping(fromMin, fromMax, std::log10(toMin), std::log10(toMax));

  return [=](float x) { return std::pow(10.0f, rawMapping(x)); };
}

} // namespace math
