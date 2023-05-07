#include "grid.hpp"

#include "constants.hpp"

void GridComponent::paint(juce::Graphics &g) {
  g.setColour(juce::Colours::black);
  g.setFont(juce::Font(getFontSize(getWidth())));

  const float circleWidth = 5.0f;
  const float halfCircleWidth = circleWidth * 0.5f;

  float w = getWidth();
  float h = getHeight();

  float labelWidth = 30.0f;

  auto freqMap = math::logMapping(constants::GRID_MIN_FREQ,
                                  constants::GRID_MAX_FREQ, 0.0f, w);
  auto dBMap = math::segmentMapping(constants::GRID_MIN_DB,
                                    constants::GRID_MAX_DB, -h, 0.0f);

  for (const auto &[freq, label] : labelFrequencies) {
    float x = freqMap(freq);

    g.setColour(juce::Colour(0xffe0e0e0));
    for (int dB = constants::GRID_MIN_DB + constants::GRID_DB_STEP;
         dB < constants::GRID_MAX_DB; dB += constants::GRID_DB_STEP) {
      float y = -dBMap(float(dB));

      g.fillEllipse(x - halfCircleWidth, y - halfCircleWidth, circleWidth,
                    circleWidth);
    }

    g.setColour(juce::Colours::black);
    g.drawText(label, int(x - labelWidth * 0.5f), int(h - labelWidth),
               int(labelWidth), int(labelWidth), juce::Justification::centred);
  }

  for (int dB = constants::GRID_MIN_DB + constants::GRID_DB_STEP;
       dB < constants::GRID_MAX_DB; dB += constants::GRID_DB_STEP) {
    float y = -dBMap(float(dB));

    g.drawText(juce::String(dB), int(labelWidth), int(y - labelWidth * 0.5f),
               int(labelWidth), int(labelWidth), juce::Justification::centred);
  }
}