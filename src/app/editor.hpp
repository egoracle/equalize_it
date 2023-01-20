#pragma once

#include "analyzer/editor.hpp"
#include "processor.hpp"
#include "shared.hpp"
#include "widgets.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

namespace app {

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
  widgets::Header header;
  AnalyzerEditor analyzerEditor;

  shared::ui::RubikFontLookAndFeel fontLookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppEditor)
};

} // namespace app