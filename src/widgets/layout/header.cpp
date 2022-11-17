#include "header.hpp"

Header::Container::Container() {
  grid.templateRows = {Track(Fr(1))};
  grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
  grid.items = {juce::GridItem(), juce::GridItem(), juce::GridItem()};

  resized();
}

Header::Header() {
  addAndMakeVisible(container);

  grid.templateRows = {Track(Fr(1))};
  grid.items = {juce::GridItem(), juce::GridItem(container), juce::GridItem()};

  resized();
}

void Header::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::white);

  g.setColour(juce::Colour::fromString("#FFEEEEEE"));
  g.drawHorizontalLine(getHeight() - 1, 0, getWidth());
}

void Header::resized() {
  grid.templateColumns = {
      Track(Fr(1)), Track(Px(juce::jmin<float>(containerWidth, getWidth()))),
      Track(Fr(1))};

  grid.performLayout(getLocalBounds());
}