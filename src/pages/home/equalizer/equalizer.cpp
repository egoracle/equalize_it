#include "equalizer.hpp"

Equalizer::Equalizer(PluginProcessor &pluginProcessor)
    : analyzer(pluginProcessor), freqResponse(pluginProcessor) {
  addAndMakeVisible(grid);
  addAndMakeVisible(analyzer);
  addAndMakeVisible(freqResponse);

  resized();
}

void Equalizer::resized() {
  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(grid)};

  LayoutComponent::resized();

  analyzer.setBounds(getLocalBounds());
  freqResponse.setBounds(getLocalBounds());
}
