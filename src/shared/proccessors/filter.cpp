#include "filter.hpp"

// FilterParameters

FilterParameters::FilterParameters(APVTS &apvts, int id) {
  filterTypeChoice = dynamic_cast<juce::AudioParameterChoice *>(
      apvts.getParameter(FilterParameters::getFilterTypeChoiceID(id)));
  isActive = dynamic_cast<juce::AudioParameterBool *>(
      apvts.getParameter(FilterParameters::getIsActiveID(id)));
  frequency = dynamic_cast<juce::AudioParameterFloat *>(
      apvts.getParameter(FilterParameters::getFrequencyID(id)));
  quality = dynamic_cast<juce::AudioParameterFloat *>(
      apvts.getParameter(FilterParameters::getQualityID(id)));
  gain = dynamic_cast<juce::AudioParameterFloat *>(
      apvts.getParameter(FilterParameters::getGainID(id)));
}

void FilterParameters::addToLayout(APVTS::ParameterLayout &layout,
                                   int filterID) {
  juce::StringArray choices{"Low Pass", "Peak", "High Pass"};
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      getFilterTypeChoiceID(filterID), getFilterTypeChoiceName(filterID),
      choices, static_cast<int>(FilterType::LowPass)));

  layout.add(std::make_unique<juce::AudioParameterBool>(
      getIsActiveID(filterID), getIsActiveName(filterID), false));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      getFrequencyID(filterID), getFrequencyName(filterID), 20.0f, 20000.0f,
      1000.0f));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      getQualityID(filterID), getQualityName(filterID), 0.1f, 10.0f, 1.0f));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      getGainID(filterID), getGainName(filterID), -12.0f, 12.0f, 0.0f));
}

juce::String FilterParameters::getFilterTypeChoiceID(int filterID) noexcept {
  return juce::String::formatted("filter_%d_type", filterID);
}

juce::String FilterParameters::getFilterTypeChoiceName(int filterID) noexcept {
  return juce::String::formatted("Filter %d Type", filterID);
}

FilterType FilterParameters::getFilterType() {
  jassert(filterTypeChoice != nullptr);

  return static_cast<FilterType>(filterTypeChoice->getIndex());
}

juce::String FilterParameters::getIsActiveID(int filterID) noexcept {
  return juce::String::formatted("filter_%d_active", filterID);
}

juce::String FilterParameters::getIsActiveName(int filterID) noexcept {
  return juce::String::formatted("Filter %d Actrive", filterID);
}

bool FilterParameters::getIsActiveValue() {
  jassert(isActive != nullptr);

  return isActive->get();
}

juce::String FilterParameters::getFrequencyID(int filterID) noexcept {
  return juce::String::formatted("filter_%d_frequency", filterID);
}

juce::String FilterParameters::getFrequencyName(int filterID) noexcept {
  return juce::String::formatted("Filter %d Frequency", filterID);
}

float FilterParameters::getFrequencyValue() {
  jassert(frequency != nullptr);

  return frequency->get();
}

juce::String FilterParameters::getQualityID(int filterID) noexcept {
  return juce::String::formatted("filter_%d_quality", filterID);
}

juce::String FilterParameters::getQualityName(int filterID) noexcept {
  return juce::String::formatted("Filter %d Quality", filterID);
}

float FilterParameters::getQualityValue() {
  jassert(quality != nullptr);

  return quality->get();
}

juce::String FilterParameters::getGainID(int filterID) noexcept {
  return juce::String::formatted("filter_%d_gain", filterID);
}

juce::String FilterParameters::getGainName(int filterID) noexcept {
  return juce::String::formatted("Filter %d Gain", filterID);
}

float FilterParameters::getGainFactorValue() {
  jassert(gain != nullptr);

  return juce::Decibels::decibelsToGain(gain->get());
}

// FilterProcessor

FilterProcessor::FilterProcessor(int filterID, APVTS &apvts)
    : id(filterID), params(apvts, filterID),
      filter(std::make_unique<MultiChannelFilter>()) {}

void FilterProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);

  juce::dsp::ProcessSpec spec;

  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = juce::uint32(samplesPerBlock);
  spec.numChannels = juce::uint32(getTotalNumOutputChannels());

  filter->reset();
  updateFilter();
  filter->prepare(spec);
}

void FilterProcessor::processBlock(juce::AudioSampleBuffer &audioBuffer,
                                   juce::MidiBuffer &midiBuffer) {
  BaseProcessor::processBlock(audioBuffer, midiBuffer);

  if (params.getIsActiveValue()) {
    juce::dsp::AudioBlock<float> audioBlock(audioBuffer);
    updateFilter();
    filter->process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
  }
}

void FilterProcessor::updateFilter() {
  auto iirCoefs = getIIRCoefficients();
  jassert(iirCoefs != nullptr);

  *filter->state = *iirCoefs;
}

FilterProcessor::IIRCoefficientsPtr FilterProcessor::getIIRCoefficients() {
  float sr = float(getSampleRate());
  float freq = params.getFrequencyValue();
  float quality = params.getQualityValue();
  float gainFactor = params.getGainFactorValue();

  IIRCoefficientsPtr iirCoefs;
  switch (params.getFilterType()) {
  case FilterType::LowPass:
    iirCoefs = IIRCoefficients::makeLowPass(sr, freq, quality);
    break;
  case FilterType::Peak:
    iirCoefs = IIRCoefficients::makePeakFilter(sr, freq, quality, gainFactor);
    break;
  case FilterType::HighPass:
    iirCoefs = IIRCoefficients::makeHighPass(sr, freq, quality);
    break;
  default:
    break;
  }

  jassert(iirCoefs != nullptr);

  return iirCoefs;
}

std::function<float(float)> FilterProcessor::getFrequencyResponse() {
  if (!params.getIsActiveValue()) {
    return BaseProcessor::getFrequencyResponse();
  }

  return [&](float freq) {
    const auto iirCoefs = this->getIIRCoefficients();
    const auto sr = this->getSampleRate();

    double mag =
        iirCoefs->getMagnitudeForFrequency(static_cast<double>(freq), sr);
    return juce::Decibels::gainToDecibels(static_cast<float>(mag));
  };
}
