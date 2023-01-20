#pragma once

#include <BinaryData.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace shared::ui {

class RubikFontLookAndFeel : public juce::LookAndFeel_V4 {
private:
  juce::Typeface::Ptr getTypefaceForFont(const juce::Font &f) override {
    return getCustomFont();
  }

public:
  RubikFontLookAndFeel() { setDefaultSansSerifTypeface(getCustomFont()); }

  static const juce::Typeface::Ptr getCustomFont() {
    static auto typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::RubikMedium_ttf, BinaryData::RubikMedium_ttfSize);

    return typeface;
  }
};

} // namespace shared::ui