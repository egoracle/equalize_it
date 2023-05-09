#pragma once

#include "lib/base_processor.hpp"

#include <juce_dsp/juce_dsp.h>

enum class FilterType { LowPass, Peak, HighPass };

struct FilterParameters {
  juce::AudioParameterChoice *filterTypeChoice;

  juce::AudioParameterBool *isActive;

  juce::AudioParameterFloat *frequency;
  juce::AudioParameterFloat *quality;
  juce::AudioParameterFloat *gain;

  static void addToLayout(APVTS::ParameterLayout &, int);

  static juce::String getFilterTypeChoiceID(int) noexcept;
  static juce::String getFilterTypeChoiceName(int) noexcept;
  FilterType getFilterType();

  static juce::String getIsActiveID(int) noexcept;
  static juce::String getIsActiveName(int) noexcept;
  bool getIsActiveValue();

  static juce::String getFrequencyID(int) noexcept;
  static juce::String getFrequencyName(int) noexcept;
  float getFrequencyValue();

  static juce::String getQualityID(int) noexcept;
  static juce::String getQualityName(int) noexcept;
  float getQualityValue();

  static juce::String getGainID(int) noexcept;
  static juce::String getGainName(int) noexcept;
  float getGainFactorValue();
};

class FilterProcessor : public BaseProcessor {
public:
  using IIRFilter = juce::dsp::IIR::Filter<float>;
  using IIRCoefficients = juce::dsp::IIR::Coefficients<float>;
  using MultiChannelFilter =
      juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefficients>;
  using IIRCoefficientsPtr = juce::ReferenceCountedObjectPtr<IIRCoefficients>;

  FilterProcessor(int, APVTS &);

  void prepareToPlay(double, int) override;
  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  IIRCoefficientsPtr getIIRCoefficients();
  std::function<float(float)> getFrequencyResponse() override;

private:
  void updateFilter();
  FilterParameters extractFilterParameters(APVTS &);

  int id;
  FilterParameters params;
  std::unique_ptr<MultiChannelFilter> filter;
};
