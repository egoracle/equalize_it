#include "gain.hpp"

#include "../constants.hpp"

GainProcessor::GainProcessor(types::APVTS &apvts) : params(apvts) {}

void GainProcessor::prepareToPlay(double sampleRate, int) {
  previousWetValueDb = params.getWetDbValue();

  rmsValueLeft.reset(sampleRate, 0.5);
  rmsValueRight.reset(sampleRate, 0.5);

  rmsValueLeft.setCurrentAndTargetValue(constants::MINUS_INFINITY_DB);
  rmsValueLeft.setCurrentAndTargetValue(constants::MINUS_INFINITY_DB);
}

void GainProcessor::processBlock(juce::AudioSampleBuffer &audioBuffer,
                                 juce::MidiBuffer &) {
  applyGain(audioBuffer);
  updateRmsValue(audioBuffer, 0);
  updateRmsValue(audioBuffer, 1);
}

float GainProcessor::getRmsValue(int channel) {
  jassert(channel == 0 || channel == 1);

  if (channel == 0) {
    return rmsValueLeft.getCurrentValue();
  }

  return rmsValueRight.getCurrentValue();
}

void GainProcessor::updateRmsValue(juce::AudioSampleBuffer &audioBuffer,
                                   int channel) {
  jassert(channel == 0 || channel == 1);

  auto &rmsValue = channel == 0 ? rmsValueLeft : rmsValueRight;

  rmsValue.skip(audioBuffer.getNumSamples());

  const auto value = juce::Decibels::gainToDecibels(
      audioBuffer.getRMSLevel(channel, 0, audioBuffer.getNumSamples()));
  if (value < rmsValue.getCurrentValue()) {
    rmsValue.setTargetValue(value);
  } else {
    rmsValue.setCurrentAndTargetValue(value);
  }
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