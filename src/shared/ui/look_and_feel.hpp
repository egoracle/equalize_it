#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class PluginLookAndFeel : public juce::LookAndFeel_V4 {
public:
  PluginLookAndFeel();

private:
  juce::Typeface::Ptr getTypefaceForFont(const juce::Font &) override;
  const juce::Typeface::Ptr getCustomFont();
};