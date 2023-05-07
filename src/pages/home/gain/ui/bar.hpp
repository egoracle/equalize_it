#pragma once

#include "shared.hpp"

#include <juce_audio_utils/juce_audio_utils.h>

class BarComponent : public juce::Component, private juce::Timer {
public:
  BarComponent(PluginProcessor &pluginProcessor, int channel)
      : pluginProcessor(pluginProcessor), channel(channel),
        level(constants::MINUS_INFINITY_DB) {
    startTimerHz(30);
  }

  void paint(juce::Graphics &) override;

  void timerCallback() override;

private:
  PluginProcessor &pluginProcessor;
  int channel;

  float level;
};