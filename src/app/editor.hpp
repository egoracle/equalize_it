#pragma once

#include "equalizer/editor.hpp"
#include "processor.hpp"
#include "widgets.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

class AppEditor : public juce::AudioProcessorEditor {
public:
  using Track = juce::Grid::TrackInfo;
  using Fr = juce::Grid::Fr;
  using Px = juce::Grid::Px;

public:
  AppEditor(AppProcessor &audioProcessor);

  ~AppEditor() override;

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  AppProcessor &audioProcessor;

  juce::Grid grid;
  Header header;
  EqualizerEditor equalizerEditor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppEditor)
};