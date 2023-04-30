#pragma once

#include "shared.hpp"

#include <map>

class GridComponent : public LayoutComponent {
public:
  GridComponent();

  void paint(juce::Graphics &) override;

private:
  const int minFreq = 5;
  const int maxFreq = 50000;

  const int minDb = -15;
  const int maxDb = 15;
  const int dBStep = 3;

  const std::map<float, juce::String> labelFrequencies{
      {20.0f, "20"},     {50.0f, "50"},    {100.0f, "100"}, {200.0f, "200"},
      {500.0f, "500"},   {1000.0f, "1k"},  {2000.0f, "2k"}, {5000.0f, "5k"},
      {10000.0f, "10k"}, {20000.0f, "20k"}};
};