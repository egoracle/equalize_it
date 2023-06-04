#pragma once

#include <juce_dsp/juce_dsp.h>

#include "../parameters/filter.hpp"
#include "../types.hpp"
#include "base_processor.hpp"

class FilterProcessor : public BaseProcessor {
public:
  using IIRFilter = juce::dsp::IIR::Filter<float>;
  using IIRCoefficients = juce::dsp::IIR::Coefficients<float>;
  using MultiChannelFilter =
      juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefficients>;
  using IIRCoefficientsPtr = juce::ReferenceCountedObjectPtr<IIRCoefficients>;

  FilterProcessor(int, types::APVTS &);

  void prepareToPlay(double, int) override;
  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  std::function<float(float)> getFrequencyResponse() override;

private:
  void updateFilter();
  IIRCoefficientsPtr getIIRCoefficients();

  int id;
  FilterParameters params;
  std::unique_ptr<MultiChannelFilter> filter;
};
