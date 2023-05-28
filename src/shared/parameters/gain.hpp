#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "../types.hpp"

struct GainParameters {
  juce::AudioParameterFloat *wet;
  static constexpr float minWetValue = -24.0f;
  static constexpr float maxWetValue = 24.0f;
  static constexpr float defaultWetValue = 0.0f;

  GainParameters(types::APVTS &apvts) {
    wet = dynamic_cast<juce::AudioParameterFloat *>(
        apvts.getParameter(GainParameters::getWetID()));
  }

  static void addToLayout(types::APVTS::ParameterLayout &layout) {
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        getWetID(), getWetName(), minWetValue, maxWetValue, defaultWetValue));
  }

  static inline juce::String getWetID() noexcept { return "gain_wet"; }
  static inline juce::String getWetName() noexcept { return "Gain Wet"; }

  float getWetDbValue() const {
    return juce::Decibels::decibelsToGain(wet->get());
  }
};