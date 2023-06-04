#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "../types.hpp"

struct FilterParameters {
  juce::AudioParameterChoice *filterTypeChoice;
  static const types::FilterType defaultFilterType = types::FilterType::Peak;

  juce::AudioParameterBool *isActive;
  static constexpr bool defaultIsActiveValue = false;

  juce::AudioParameterFloat *quality;
  static constexpr float minQualityValue = 0.1f;
  static constexpr float maxQualityValue = 10.0f;
  static constexpr float defaultQualityValue = 1.0f;

  juce::AudioParameterFloat *frequency;
  static constexpr float minFrequencyValue = 20.0f;
  static constexpr float maxFrequencyValue = 20000.0f;
  static constexpr float defaultFrequencyValue = 1000.0f;

  juce::AudioParameterFloat *gain;
  static constexpr float minGainValue = -12.0f;
  static constexpr float maxGainValue = 12.0f;
  static constexpr float defaultGainValue = 0.0f;

  FilterParameters(int filterID, types::APVTS &apvts) {
    filterTypeChoice = dynamic_cast<juce::AudioParameterChoice *>(
        apvts.getParameter(FilterParameters::getFilterTypeChoiceID(filterID)));
    isActive = dynamic_cast<juce::AudioParameterBool *>(
        apvts.getParameter(FilterParameters::getIsActiveID(filterID)));
    frequency = dynamic_cast<juce::AudioParameterFloat *>(
        apvts.getParameter(FilterParameters::getFrequencyID(filterID)));
    quality = dynamic_cast<juce::AudioParameterFloat *>(
        apvts.getParameter(FilterParameters::getQualityID(filterID)));
    gain = dynamic_cast<juce::AudioParameterFloat *>(
        apvts.getParameter(FilterParameters::getGainID(filterID)));
  }

  static void addToLayout(int filterID, types::APVTS::ParameterLayout &layout) {
    juce::StringArray choices{"Low Pass", "Peak", "High Pass"};
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        getFilterTypeChoiceID(filterID), getFilterTypeChoiceName(filterID),
        choices, static_cast<int>(defaultFilterType)));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        getIsActiveID(filterID), getIsActiveName(filterID),
        defaultIsActiveValue));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        getQualityID(filterID), getQualityName(filterID), minQualityValue,
        maxQualityValue, defaultQualityValue));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        getFrequencyID(filterID), getFrequencyName(filterID),
        juce::NormalisableRange<float>(minFrequencyValue, maxFrequencyValue,
                                       0.01f, 0.2f),
        defaultFrequencyValue));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        getGainID(filterID), getGainName(filterID), minGainValue, maxGainValue,
        defaultGainValue));
  }

  static juce::String getFilterTypeChoiceID(int filterID) noexcept {
    return juce::String::formatted("filter_%d_type", filterID);
  }
  static juce::String getFilterTypeChoiceName(int filterID) noexcept {
    return juce::String::formatted("Filter %d Type", filterID);
  }
  types::FilterType getFilterType() const {
    return static_cast<types::FilterType>(filterTypeChoice->getIndex());
  }

  static juce::String getIsActiveID(int filterID) noexcept {
    return juce::String::formatted("filter_%d_active", filterID);
  }
  static juce::String getIsActiveName(int filterID) noexcept {
    return juce::String::formatted("Filter %d Actrive", filterID);
  }
  bool getIsActiveValue() const { return isActive->get(); }

  static juce::String getQualityID(int filterID) noexcept {
    return juce::String::formatted("filter_%d_quality", filterID);
  }
  static juce::String getQualityName(int filterID) noexcept {
    return juce::String::formatted("Filter %d Quality", filterID);
  }
  float getQualityValue() const { return quality->get(); }

  static juce::String getFrequencyID(int filterID) noexcept {
    return juce::String::formatted("filter_%d_frequency", filterID);
  }
  static juce::String getFrequencyName(int filterID) noexcept {
    return juce::String::formatted("Filter %d Frequency", filterID);
  }
  float getFrequencyValue() const { return frequency->get(); }

  static juce::String getGainID(int filterID) noexcept {
    return juce::String::formatted("filter_%d_gain", filterID);
  }
  static juce::String getGainName(int filterID) noexcept {
    return juce::String::formatted("Filter %d Gain", filterID);
  }
  float getGainValue() const {
    return juce::Decibels::decibelsToGain(gain->get());
  }
};