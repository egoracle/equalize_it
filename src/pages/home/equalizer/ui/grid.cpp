#include "grid.hpp"

GridComponent::GridComponent() {}

void GridComponent::paint(juce::Graphics &g) {
  g.setColour(juce::Colours::black);

  float w = float(getWidth()) - 1;
  float h = float(getHeight()) - 1;

  float labelWidth = 30.0f;

  auto freqMap = math::logMapping(float(minFreq), float(maxFreq), 0.0f, w);
  auto dBMap = math::segmentMapping(float(minDb), float(maxDb), -h, 0.0f);

  for (const auto &[freq, label] : labelFrequencies) {
    float x = freqMap(freq);

    for (int dB = minDb + dBStep; dB < maxDb; dB += dBStep) {
      float y = -dBMap(float(dB));

      g.drawEllipse(x - 5, y - 5, 10.0f, 10.0f, 3);
    }

    g.drawText(label, int(x - labelWidth * 0.5f), int(h - labelWidth),
               int(labelWidth), int(labelWidth), juce::Justification::centred);
  }

  for (int dB = minDb + dBStep; dB < maxDb; dB += dBStep) {
    float y = -dBMap(float(dB));

    g.drawText(juce::String(dB), int(labelWidth), int(y - labelWidth * 0.5f),
               int(labelWidth), int(labelWidth), juce::Justification::centred);
  }
}