#pragma once

#include "shared.hpp"

#include <functional>

class EqualizerFrequencyResponse : public juce::Component, private juce::Timer {
public:
  EqualizerFrequencyResponse(PluginProcessor &);

  void paint(juce::Graphics &) override;

  void timerCallback() override;

private:
  PluginProcessor &pluginProcessor;
  std::function<float(float)> frequencyResponse;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerFrequencyResponse)
};