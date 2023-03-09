#include "home.hpp"

HomePage::HomePage() : LayoutComponent() {
  addAndMakeVisible(equalizer);
  addAndMakeVisible(header);

  resized();
}

void HomePage::paint(juce::Graphics &g) { g.fillAll(juce::Colour(0xfffafcfc)); }

void HomePage::resized() {
  layout.templateRows = {Track(Px(headerHeight)), Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(header), juce::GridItem(equalizer)};

  LayoutComponent::resized();
}