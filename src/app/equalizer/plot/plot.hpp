#pragma once

#include "../../../shared/shared.hpp"
#include "../processor.hpp"

#include <map>

class EqualizerPlot : public BaseComponent {
public:
  class PlotGrid : public BaseComponent {
  public:
    PlotGrid();

    void paint(juce::Graphics &g);

  private:
    std::map<int, float> xLogFrequencies;
  };

  class PlotAxisX : public BaseComponent {
  public:
    PlotAxisX();

    void paint(juce::Graphics &g);

  private:
    std::map<int, juce::String> labelFrequencies;
  };

public:
  EqualizerPlot(BaseProcessor &audioProcessor);

private:
  BaseProcessor &audioProcessor;

  PlotGrid plotGrid;
  PlotAxisX plotAxisX;
};
