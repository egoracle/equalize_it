#pragma once

#include "pages.hpp"
#include "shared.hpp"

class PluginEditor : public juce::AudioProcessorEditor {
public:
  enum {
    minWidth = 720,
    minHeight = 480,
    defaultWidth = 1280,
    defaultHeight = 720
  };

public:
  explicit PluginEditor(PluginProcessor &);
  ~PluginEditor() override;

  void resized() override;

private:
  SairaFontLookAndFeel lookAndFeel;

  HomePage homePage;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
