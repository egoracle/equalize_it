#include "gain.hpp"

GainProcessor::GainProcessor(juce::AudioProcessorValueTreeState &state)
    : apvts(state), rmsValueLeft(-70.0f), rmsValueRight(-70.0f) {}

void GainProcessor::prepareToPlay(double, int) {
  previousGainValue = *apvts.getRawParameterValue("gain");
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
  float currentGainValue = *apvts.getRawParameterValue("gain");

  if (currentGainValue == previousGainValue) {
    audioBuffer.applyGain(juce::Decibels::decibelsToGain(currentGainValue));
  } else {
    audioBuffer.applyGainRamp(0, audioBuffer.getNumSamples(),
                              juce::Decibels::decibelsToGain(previousGainValue),
                              juce::Decibels::decibelsToGain(currentGainValue));
    previousGainValue = currentGainValue;
  }
}