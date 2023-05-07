#pragma once

#include "shared.hpp"

#include <functional>
#include <juce_audio_utils/juce_audio_utils.h>

class AnalyzerComponent : public LayoutComponent, private juce::Timer {
public:
  AnalyzerComponent(PluginProcessor &);

  void paint(juce::Graphics &) override;

  void timerCallback() override;

private:
  PluginProcessor &pluginProcessor;
};