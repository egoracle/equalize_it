#pragma once

#include "shared.hpp"

#include <map>

class GridComponent : public LayoutComponent {
public:
  GridComponent()
      : fontLinInterpCoef((maxFontSize - minFontSize) / (maxWidth - minWidth)) {
  }

  void paint(juce::Graphics &) override;

private:
  float getFontSize(float width) {
    return minFontSize + fontLinInterpCoef * (width - minWidth);
  }

  const float minFontSize = 14.0f;
  const float maxFontSize = 20.0f;
  const float minWidth = 720.0f;
  const float maxWidth = 1920.0f;
  float fontLinInterpCoef;

  const std::map<float, juce::String> labelFrequencies{
      {20.0f, "20"},     {50.0f, "50"},    {100.0f, "100"}, {200.0f, "200"},
      {500.0f, "500"},   {1000.0f, "1k"},  {2000.0f, "2k"}, {5000.0f, "5k"},
      {10000.0f, "10k"}, {20000.0f, "20k"}};
};