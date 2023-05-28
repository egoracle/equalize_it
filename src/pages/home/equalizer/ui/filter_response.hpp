#pragma once

#include "shared.hpp"

#include <functional>

class FilterFrequencyResponse : public juce::Component, private juce::Timer {
public:
  FilterFrequencyResponse(PluginProcessor &, int);

  void paint(juce::Graphics &) override;
  void resized() override {}

  void timerCallback() override;

  std::function<float(float)> &getFrequencyResponse();

private:
  PluginProcessor &pluginProcessor;
  int filterID;

  juce::Colour colour;
  FilterParameters parameters;
  std::function<float(float)> frequencyResponse;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterFrequencyResponse)
};