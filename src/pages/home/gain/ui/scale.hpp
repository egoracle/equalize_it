#pragma once

#include "shared.hpp"

#include <vector>

class ScaleComponent : public LayoutComponent {
public:
  ScaleComponent()
      : fontLinInterpCoef((maxFontSize - minFontSize) /
                          (maxHeight - minHeight)) {}

  void paint(juce::Graphics &g) override;

private:
  float getFontSize(float height) {
    return minFontSize + fontLinInterpCoef * (height - minHeight);
  }

  std::vector<int> labels{0,   -3,  -6,  -9,  -12, -18, -24,
                          -30, -36, -48, -60, -72, -84};

  const float minFontSize = 14.0f;
  const float maxFontSize = 20.0f;
  const float minHeight = 480.0f;
  const float maxHeight = 1080.0f;
  float fontLinInterpCoef;
};