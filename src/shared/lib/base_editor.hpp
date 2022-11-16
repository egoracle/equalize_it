#pragma once

#include "base_processor.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

class BaseEditor : public juce::AudioProcessorEditor {
public:
  using Track = juce::Grid::TrackInfo;
  using Fr = juce::Grid::Fr;
  using Px = juce::Grid::Px;

public:
  BaseEditor(BaseProcessor &audioProcessor)
      : juce::AudioProcessorEditor(&audioProcessor),
        audioProcessor(audioProcessor) {}
  virtual ~BaseEditor() {}

  virtual void paint(juce::Graphics &g) {}
  virtual void resized() { grid.performLayout(getLocalBounds()); }

protected:
  BaseProcessor &audioProcessor;
  juce::Grid grid;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseEditor)
};