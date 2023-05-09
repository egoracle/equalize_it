#pragma once

#include "shared.hpp"

#include <functional>

class FilterComponent : public juce::Component, private juce::Timer {
public:
  FilterComponent(PluginProcessor &, int);

  void paint(juce::Graphics &) override;

  void timerCallback() override;

private:
  PluginProcessor &pluginProcessor;
  int filterID;
  juce::Colour colour;
  FilterParameters parameters;
  std::function<float(float)> frequencyResponse;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};