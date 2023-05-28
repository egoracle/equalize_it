#include "equalizer.hpp"

#include <algorithm>

Equalizer::Equalizer(PluginProcessor &p)
    : pluginProcessor(p), uiState(p.getUiState()), analyzer(p), freqResponse(p),
      filterPanel(p, [&]() { update(); }) {
  addMouseListener(this, true);

  addAndMakeVisible(grid);
  addAndMakeVisible(analyzer);

  for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
       ++id) {
    filterFreqResponses.push_back(
        std::make_unique<FilterFrequencyResponse>(pluginProcessor, id));
    addChildComponent(filterFreqResponses.back().get());
  }

  addAndMakeVisible(freqResponse);

  for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
       ++id) {
    filterButtons.push_back(std::make_unique<FilterButton>(
        id, pluginProcessor, [&]() { update(); }));
    addChildComponent(filterButtons.back().get());
  }

  addChildComponent(filterPanel);

  update();

  resized();
}

void Equalizer::resized() {
  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(grid)};

  LayoutComponent::resized();

  const auto bounds = getLocalBounds();

  analyzer.setBounds(bounds);
  for (auto &filterFreqResponse : filterFreqResponses) {
    filterFreqResponse->setBounds(bounds);
  }
  freqResponse.setBounds(bounds);
  filterPanel.setBounds(bounds.withSizeKeepingCentre(330, 170));
  for (auto &filterButton : filterButtons) {
    filterButton->timerCallback();
  }
}

void Equalizer::update() {
  filterPanel.update();
  for (const auto &[filterID, isUsed] : uiState.usedFilterIDs) {
    filterFreqResponses[filterID - 1]->setVisible(isUsed);
    filterButtons[filterID - 1]->setVisible(isUsed);
  }
}
