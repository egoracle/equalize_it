#include "bar.hpp"

#include "constants.hpp"

void BarComponent::paint(juce::Graphics &g) {
  auto bounds = getLocalBounds().toFloat();

  auto dBMap = math::segmentMapping(
      constants::GAIN_MIN_DB, constants::GAIN_MAX_DB, 0.0f, float(getHeight()));
  const auto scaledY = dBMap(level);

  g.setGradientFill(juce::ColourGradient(
      juce::Colour(0xff87bfff), bounds.getBottomLeft(),
      juce::Colour(0xff2683ee), bounds.getTopLeft(), false));
  g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.0f);
}

void BarComponent::timerCallback() {
  auto gainProcessor = pluginProcessor.getGainProcessor();

  if (gainProcessor) {
    level = gainProcessor->getRmsValue(channel);
  }

  repaint();
}