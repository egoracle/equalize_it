#pragma once

#include "../processor.hpp"
#include "shared.hpp"
#include "spectrum.hpp"

#include <map>

class EqualizerPlot : public BaseComponent {
public:
  class GridPlot : public BaseComponent {
  public:
    GridPlot();

    void paint(juce::Graphics &g);

  private:
    std::map<int, float> xLogFrequencies;
  };

  class AxisXPlot : public BaseComponent {
  public:
    AxisXPlot();

    void paint(juce::Graphics &g);

  private:
    std::map<int, juce::String> labelFrequencies;
  };

public:
  EqualizerPlot(BaseProcessor &audioProcessor);

  void resized() override;

private:
  BaseProcessor &audioProcessor;

  GridPlot gridPlot;
  AxisXPlot axisXPlot;
  SpectrumPlot spectrumPlot;

  juce::Grid spectrumGrid;
};
