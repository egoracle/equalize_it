#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "pages.hpp"
#include "processor.hpp"

class AppEditor : public juce::AudioProcessorEditor {
public:
  enum {
    minWidth = 720,
    minHeight = 480,
    defaultWidth = 1280,
    defaultHeight = 720
  };

public:
  explicit AppEditor(AppProcessor &);
  ~AppEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  HomePage homePage;
  AppProcessor &appProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppEditor)
};
