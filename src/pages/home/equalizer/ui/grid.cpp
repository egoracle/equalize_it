#include "grid.hpp"

void GridComponent::paint(juce::Graphics &g) {
  g.setColour(juce::Colours::black);
  g.setFont(juce::Font(getFontSize(getWidth())));

  const float circleWidth = 5.0f;
  const float halfCircleWidth = circleWidth * 0.5f;

  float w = float(getWidth()) - 1;
  float h = float(getHeight()) - 1;

  float labelWidth = 30.0f;

  auto freqMap = math::logMapping(float(minFreq), float(maxFreq), 0.0f, w);
  auto dBMap = math::segmentMapping(float(minDb), float(maxDb), -h, 0.0f);

  for (const auto &[freq, label] : labelFrequencies) {
    float x = freqMap(freq);

    g.setColour(juce::Colour(0xffe0e0e0));
    for (int dB = minDb + dBStep; dB < maxDb; dB += dBStep) {
      float y = -dBMap(float(dB));

      g.fillEllipse(x - halfCircleWidth, y - halfCircleWidth, circleWidth,
                    circleWidth);
    }

    g.setColour(juce::Colours::black);
    g.drawText(label, int(x - labelWidth * 0.5f), int(h - labelWidth),
               int(labelWidth), int(labelWidth), juce::Justification::centred);
  }

  for (int dB = minDb + dBStep; dB < maxDb; dB += dBStep) {
    float y = -dBMap(float(dB));

    g.drawText(juce::String(dB), int(labelWidth), int(y - labelWidth * 0.5f),
               int(labelWidth), int(labelWidth), juce::Justification::centred);
  }
}