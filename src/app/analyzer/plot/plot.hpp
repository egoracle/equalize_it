#pragma once

#include "../processor.hpp"
#include "shared.hpp"
#include "spectrum.hpp"

#include <map>

class AnalyzerPlot : public shared::lib::BaseComponent {
public:
  class GridPlot : public shared::lib::BaseComponent {
  public:
    GridPlot();

    void paint(juce::Graphics &g);

  private:
    std::map<int, float> xLogFrequencies;
  };

  class AxisXPlot : public shared::lib::BaseComponent {
  public:
    AxisXPlot();

    void paint(juce::Graphics &g);

  private:
    std::map<int, juce::String> labelFrequencies;
  };

  class AxisYPlot : public BaseComponent {
  public:
    AxisYPlot(){};

    void paint(juce::Graphics &g);
  };

public:
  AnalyzerPlot(shared::lib::BaseProcessor &audioProcessor);

  void resized() override;

private:
  shared::lib::BaseProcessor &audioProcessor;

  GridPlot gridPlot;
  AxisXPlot axisXPlot;
  AxisYPlot axisYPlot;
  SpectrumPlot spectrumPlot;

  juce::Grid spectrumGrid;
};
