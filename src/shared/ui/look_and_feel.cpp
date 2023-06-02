#include "look_and_feel.hpp"

#include <BinaryData.h>
#include <cmath>
#include <numbers>

#include "colours.hpp"

PluginLookAndFeel::PluginLookAndFeel() {
  setDefaultSansSerifTypeface(getCustomFont());
}

juce::Typeface::Ptr PluginLookAndFeel::getTypefaceForFont(const juce::Font &) {
  return getCustomFont();
}

const juce::Typeface::Ptr PluginLookAndFeel::getCustomFont() {
  return juce::Typeface::createSystemTypefaceFor(
      BinaryData::SairaMedium_ttf, BinaryData::SairaMedium_ttfSize);
}