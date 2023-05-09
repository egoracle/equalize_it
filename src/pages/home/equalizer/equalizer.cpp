#include "equalizer.hpp"

Equalizer::Equalizer(PluginProcessor &pluginProcessor)
    : analyzer(pluginProcessor), freqResponse(pluginProcessor) {
  addAndMakeVisible(grid);
  addAndMakeVisible(analyzer);

  for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
       ++id) {
    filters.push_back(std::make_unique<FilterComponent>(pluginProcessor, id));
    addAndMakeVisible(*filters.back());
  }

  addAndMakeVisible(freqResponse);

  resized();
}

void Equalizer::resized() {
  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(grid)};

  LayoutComponent::resized();

  const auto bounds = getLocalBounds();
  analyzer.setBounds(bounds);
  freqResponse.setBounds(bounds);
  for (auto &filter : filters) {
    filter->setBounds(bounds);
  }
}
