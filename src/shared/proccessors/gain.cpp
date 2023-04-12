#include "gain.hpp"

// GainParameters

void GainParameters::addToLayout(APVTS::ParameterLayout &layout) {
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      getWetID(), getWetName(), -12.0f, 12.0f, 0.0f));
}

inline juce::String GainParameters::getWetID() noexcept { return "gain_wet"; }

inline juce::String GainParameters::getWetName() noexcept { return "Gain Wet"; }

float GainParameters::getWetDbValue() {
  jassert(wet != nullptr);

  return juce::Decibels::decibelsToGain(wet->get());
}

// GainProcessor

GainProcessor::GainProcessor(APVTS &apvts)
    : params(extractGainParameters(apvts)) {}

void GainProcessor::prepareToPlay(double, int) {
  previousWetValueDb = params.getWetDbValue();
}

void GainProcessor::processBlock(juce::AudioSampleBuffer &audioBuffer,
                                 juce::MidiBuffer &) {
  applyGain(audioBuffer);
  updateRmsValues(audioBuffer);
}

float GainProcessor::getRmsValue(int channel) {
  jassert(channel == 0 || channel == 1);

  if (channel == 0) {
    return rmsValueLeft;
  }

  return rmsValueRight;
}

void GainProcessor::updateRmsValues(juce::AudioSampleBuffer &audioBuffer) {
  rmsValueLeft = juce::Decibels::gainToDecibels(
      audioBuffer.getRMSLevel(0, 0, audioBuffer.getNumSamples()));
  rmsValueRight = juce::Decibels::gainToDecibels(
      audioBuffer.getRMSLevel(1, 0, audioBuffer.getNumSamples()));
}

void GainProcessor::applyGain(juce::AudioSampleBuffer &audioBuffer) {
  float currentWetValueDb = params.getWetDbValue();

  if (currentWetValueDb == previousWetValueDb) {
    audioBuffer.applyGain(currentWetValueDb);
  } else {
    audioBuffer.applyGainRamp(0, audioBuffer.getNumSamples(),
                              previousWetValueDb, currentWetValueDb);
    previousWetValueDb = currentWetValueDb;
  }
}

GainParameters GainProcessor::extractGainParameters(APVTS &apvts) {
  GainParameters params;
  params.wet = dynamic_cast<juce::AudioParameterFloat *>(
      apvts.getParameter(GainParameters::getWetID()));

  return params;
}