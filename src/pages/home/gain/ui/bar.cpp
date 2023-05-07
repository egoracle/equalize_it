#include "bar.hpp"

void BarComponent::paint(juce::Graphics &g) {
  auto bounds = getLocalBounds().toFloat();

  g.setColour(juce::Colours::black);
  g.fillRoundedRectangle(bounds, 5.0f);

  g.setColour(juce::Colour(0xff2683ee));
  const auto scaledY =
      juce::jmap(level, -70.0f, 6.0f, 0.0f, static_cast<float>(getHeight()));
  g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.0f);
}

void BarComponent::timerCallback() {
  auto gainProcessor = pluginProcessor.getGainProcessor();

  if (gainProcessor) {
    level = gainProcessor->getRmsValue(channel);
  }

  repaint();
}