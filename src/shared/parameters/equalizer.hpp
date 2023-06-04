#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "../constants.hpp"
#include "../types.hpp"
#include "filter.hpp"

struct EqualizerParameters {
  juce::AudioParameterBool *isActive;
  static constexpr bool defaultIsActiveValue = true;

  EqualizerParameters(types::APVTS &apvts) {
    isActive = dynamic_cast<juce::AudioParameterBool *>(
        apvts.getParameter(EqualizerParameters::getIsActiveID()));
  }

  static void addToLayout(types::APVTS::ParameterLayout &layout) {
    layout.add(std::make_unique<juce::AudioParameterBool>(
        getIsActiveID(), getIsActiveName(), defaultIsActiveValue));

    for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
         ++id) {
      FilterParameters::addToLayout(id, layout);
    }
  }

  static inline juce::String getIsActiveID() noexcept {
    return "equalizer_active";
  }
  static inline juce::String getIsActiveName() noexcept {
    return "Equalizer Active";
  }
  bool getIsActiveValue() const { return isActive->get(); }
};