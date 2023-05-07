#include "scale.hpp"

#include "constants.hpp"

void ScaleComponent::paint(juce::Graphics &g) {
  const float lineThickness = 2.0f;

  float xMax = float(getWidth());
  float xCenter = xMax * 0.5f;
  float yMax = float(getHeight());

  juce::Line<float> verticalLine(juce::Point<float>(xCenter, 0),
                                 juce::Point<float>(xCenter, yMax));
  g.setColour(juce::Colours::black);
  g.drawLine(verticalLine, lineThickness);

  float fontSize = getFontSize(yMax);
  g.setFont(juce::Font(fontSize));

  float labelWidth = fontSize;
  float halfLabelWidth = float(labelWidth) * 0.5f;

  float labelHeight = 15;
  float halfLabelHeight = float(labelHeight) * 0.5f;

  auto dBMap = math::segmentMapping(constants::GAIN_MIN_DB,
                                    constants::GAIN_MAX_DB, -yMax, 0.0f);

  for (size_t i = 0; i < labels.size() - 1; ++i) {
    float y = -dBMap(float(labels[i]));

    juce::Line<float> horizontalLine(
        juce::Point<float>(xCenter - labelWidth * 0.75f, y),
        juce::Point<float>(xCenter + labelWidth * 0.75f, y));
    g.setColour(juce::Colours::black);
    g.drawLine(horizontalLine, lineThickness);

    juce::Rectangle<float> textRect(
        xCenter - halfLabelWidth, y - halfLabelHeight, labelWidth, labelHeight);

    g.setColour(juce::Colours::white);
    g.fillRect(textRect);

    g.setColour(juce::Colours::black);
    g.drawText(juce::String(-labels[i]), textRect,
               juce::Justification::centred);
  }
}