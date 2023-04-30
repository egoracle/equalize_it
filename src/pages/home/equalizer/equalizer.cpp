#include "equalizer.hpp"

Equalizer::Equalizer(PluginProcessor &pluginProcessor)
    : analyzer(pluginProcessor) {
  addAndMakeVisible(grid);
  addAndMakeVisible(analyzer);

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

  analyzer.setBounds(getLocalBounds());
}
