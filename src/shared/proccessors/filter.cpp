#include "filter.hpp"

FilterProcessor::FilterProcessor(int filterID, types::APVTS &apvts)
    : id(filterID), params(filterID, apvts),
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
  float gainFactor = params.getGainValue();

  IIRCoefficientsPtr iirCoefs;
  switch (params.getFilterType()) {
  case types::FilterType::LowPass:
    iirCoefs = IIRCoefficients::makeLowPass(sr, freq, quality);
    break;
  case types::FilterType::Peak:
    iirCoefs = IIRCoefficients::makePeakFilter(sr, freq, quality, gainFactor);
    break;
  case types::FilterType::HighPass:
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

  const auto iirCoefs = getIIRCoefficients();
  const double sr = getSampleRate();

  return [&, iirCoefs, sr](float freq) {
    double mag =
        iirCoefs->getMagnitudeForFrequency(static_cast<double>(freq), sr);
    return juce::Decibels::gainToDecibels(static_cast<float>(mag));
  };
}
