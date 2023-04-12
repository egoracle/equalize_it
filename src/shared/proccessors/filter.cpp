#include "filter.hpp"

// FilterParameters

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
      getPassPeakID(filterID), getPassPeakName(filterID), 0.1f, 10.0f, 1.0f));
}

juce::String FilterParameters::getFilterTypeChoiceID(int filterID) noexcept {
  return juce::String::formatted("filter_type_%d", filterID);
}

juce::String FilterParameters::getFilterTypeChoiceName(int filterID) noexcept {
  return juce::String::formatted("Filter Type %d", filterID);
}

FilterType FilterParameters::getFilterType() {
  jassert(filterTypeChoice != nullptr);

  return static_cast<FilterType>(filterTypeChoice->getIndex());
}

juce::String FilterParameters::getIsActiveID(int filterID) noexcept {
  return juce::String::formatted("filter_active_%d", filterID);
}

juce::String FilterParameters::getIsActiveName(int filterID) noexcept {
  return juce::String::formatted("Filter Actrive %d", filterID);
}

bool FilterParameters::getIsActiveValue() {
  jassert(isActive != nullptr);

  return isActive->get();
}

juce::String FilterParameters::getFrequencyID(int filterID) noexcept {
  return juce::String::formatted("filter_frequency_%d", filterID);
}

juce::String FilterParameters::getFrequencyName(int filterID) noexcept {
  return juce::String::formatted("Filter Frequency %d", filterID);
}

float FilterParameters::getFrequencyValue() {
  jassert(frequency != nullptr);

  return frequency->get();
}

juce::String FilterParameters::getPassPeakID(int filterID) noexcept {
  return juce::String::formatted("filter_pass_peak_%d", filterID);
}

juce::String FilterParameters::getPassPeakName(int filterID) noexcept {
  return juce::String::formatted("Filter Pass Peak %d", filterID);
}

float FilterParameters::getPassPeakValue() {
  jassert(passPeak != nullptr);

  return passPeak->get();
}

// FilterProcessor

FilterProcessor::FilterProcessor(int filterID, APVTS &apvts)
    : id(filterID), params(extractFilterParameters(apvts)),
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
  float freq = params.getFrequencyValue();
  float res = params.getPassPeakValue();

  IIRCoefficientsPtr iirCoefs;
  switch (params.getFilterType()) {
  case FilterType::LowPass:
    iirCoefs = IIRCoefficients::makeLowPass(getSampleRate(), freq, res);
    break;
  case FilterType::Peak:
    // TODO
    break;
  case FilterType::HighPass:
    iirCoefs = IIRCoefficients::makeHighPass(getSampleRate(), freq, res);
    break;
  default:
    break;
  }

  jassert(iirCoefs != nullptr);

  return iirCoefs;
}

FilterParameters FilterProcessor::extractFilterParameters(APVTS &apvts) {
  FilterParameters params;

  params.filterTypeChoice = dynamic_cast<juce::AudioParameterChoice *>(
      apvts.getParameter(FilterParameters::getFilterTypeChoiceID(id)));
  params.isActive = dynamic_cast<juce::AudioParameterBool *>(
      apvts.getParameter(FilterParameters::getIsActiveID(id)));
  params.frequency = dynamic_cast<juce::AudioParameterFloat *>(
      apvts.getParameter(FilterParameters::getFrequencyID(id)));
  params.passPeak = dynamic_cast<juce::AudioParameterFloat *>(
      apvts.getParameter(FilterParameters::getPassPeakID(id)));

  return params;
}
