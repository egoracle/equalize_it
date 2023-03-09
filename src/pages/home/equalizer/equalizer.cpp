#include "equalizer.hpp"

Equalizer::Equalizer() : LayoutComponent() {
  addAndMakeVisible(grid);

  resized();
}

void Equalizer::paint(juce::Graphics &g) {
  // g.fillAll(juce::Colour(0xfff6feff));
}

void Equalizer::resized() {
  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(grid)};

  LayoutComponent::resized();
}
