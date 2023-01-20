#include "average_points.hpp"

std::vector<float>
shared::core::graphics::averagePoints(std::vector<float> &x,
                                      std::vector<float> &y,
                                      std::vector<float> &newX, int height) {
  std::vector<float> newY;

  int startIndex = 0;
  float prevSmoothY = height;

  for (int i = 0; i < newX.size() - 1; i++) {
    float yCumulative = 0;
    float yCount = 0;

    int currentIndex = startIndex;
    while (x[currentIndex] < newX[i]) {
      yCumulative += y[currentIndex];
      yCount++;
      currentIndex++;
    }

    if (yCount != 0) {
      prevSmoothY = yCumulative / yCount;
      newY.push_back(prevSmoothY);
    } else {
      newY.push_back(prevSmoothY);
    }

    startIndex = currentIndex;
  }

  newY.push_back(height);

  return newY;
}