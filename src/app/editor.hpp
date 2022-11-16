#pragma once

#include "processor.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

class AppEditor : public juce::AudioProcessorEditor {
public:
  AppEditor(AppProcessor &audioProcessor);
  ~AppEditor() override;

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  AppProcessor &audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppEditor)
};