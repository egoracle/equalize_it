#pragma once

#include "shared.hpp"

#include <juce_audio_utils/juce_audio_utils.h>

class LevelMeter : public juce::Component, private juce::Timer {
public:
  LevelMeter(int, PluginProcessor &);

  void paint(juce::Graphics &) override;

  void timerCallback() override;

private:
  int channel;

  PluginProcessor &pluginProcessor;

  float level;
};