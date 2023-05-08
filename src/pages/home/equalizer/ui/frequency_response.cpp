#include "frequency_response.hpp"

#include "constants.hpp"

FrequencyResponseComponent::FrequencyResponseComponent(PluginProcessor &p)
    : pluginProcessor(p), frequencyResponse([](float) { return 0.0f; }) {
  startTimerHz(30);
}

void FrequencyResponseComponent::paint(juce::Graphics &g) {
  const auto equalizerProcessor = pluginProcessor.getEqualizerProcessor();

  if (!equalizerProcessor) {
    return;
  }

  const float xMin = 0.0f;
  float xMax = static_cast<float>(getWidth());
  const float yMax = static_cast<float>(getHeight());

  const float halfSampleRate =
      static_cast<float>(equalizerProcessor->getSampleRate() * 0.5);

  const auto xToFreq = math::invLogMapping(xMin, xMax, constants::GRID_MIN_FREQ,
                                           constants::GRID_MAX_FREQ);
  const auto dBToY = math::segmentMapping(constants::GRID_MIN_DB,
                                          constants::GRID_MAX_DB, yMax, 0.0f);

  juce::Path freqRespPath;
  freqRespPath.preallocateSpace(3 * static_cast<int>(xMax));

  freqRespPath.startNewSubPath(xMin, dBToY(frequencyResponse(xMin)));
  for (float x = xMin + 1; x < xMax; ++x) {
    const float freq = xToFreq(x);
    if (freq > halfSampleRate) {
      break;
    }
    freqRespPath.lineTo(x, dBToY(frequencyResponse(freq)));
  }

  g.setColour(juce::Colour(0xffff5050));
  g.strokePath(freqRespPath, juce::PathStrokeType(
                                 2.f, juce::PathStrokeType::JointStyle::curved,
                                 juce::PathStrokeType::EndCapStyle::rounded));
}

void FrequencyResponseComponent::timerCallback() {
  const auto equalizerProcessor = pluginProcessor.getEqualizerProcessor();

  if (equalizerProcessor) {
    frequencyResponse = equalizerProcessor->getFrequencyResponse();
  }
}