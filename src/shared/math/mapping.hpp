#pragma once

#include <functional>

namespace math {

std::function<float(float)> segmentMapping(float, float, float, float);

std::function<float(float)> logMapping(float, float, float, float);

std::function<float(float)> invLogMapping(float, float, float, float);

} // namespace math
