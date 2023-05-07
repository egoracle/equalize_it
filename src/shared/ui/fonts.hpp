#pragma once

#include <BinaryData.h>
#include <juce_gui_basics/juce_gui_basics.h>

class SairaFontLookAndFeel : public juce::LookAndFeel_V4 {
public:
  SairaFontLookAndFeel() { setDefaultSansSerifTypeface(getCustomFont()); }

  static const juce::Typeface::Ptr getCustomFont() {
    static auto typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::SairaMedium_ttf, BinaryData::SairaMedium_ttfSize);

    return typeface;
  }

private:
  juce::Typeface::Ptr getTypefaceForFont(const juce::Font &f) override {
    return getCustomFont();
  }
};