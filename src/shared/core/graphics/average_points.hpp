#pragma once

#include <vector>

namespace shared::core::graphics {

std::vector<float> averagePoints(std::vector<float> &x, std::vector<float> &y,
                                 std::vector<float> &newX, int height);

} // namespace shared::core::graphics