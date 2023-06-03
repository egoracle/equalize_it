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

void Equalizer::mouseDown(const juce::MouseEvent &event) {
  LayoutComponent::mouseDown(event);

  if (event.eventComponent == &freqResponse) {
    uiState.selectedFilterID = -1;
    update();
  }
}

void Equalizer::mouseDoubleClick(const juce::MouseEvent &event) {
  LayoutComponent::mouseDoubleClick(event);

  if (event.eventComponent != &freqResponse || !uiState.addFilter()) {
    return;
  }

  const float xMin = 0.0f;
  const float xMax = static_cast<float>(event.eventComponent->getWidth());
  const float yMax = static_cast<float>(event.eventComponent->getHeight());

  const auto xToFreq = math::invLogMapping(xMin, xMax, constants::GRID_MIN_FREQ,
                                           constants::GRID_MAX_FREQ);
  const auto freqToNorm = math::segmentMapping(20, 20000, 0, 1);
  const auto yToDb = math::segmentMapping(yMax, 0.0f, constants::GRID_MIN_DB,
                                          constants::GRID_MAX_DB);
  const auto dBToNorm = math::segmentMapping(-12, 12, 0, 1);

  const int x = event.getMouseDownX();
  const int y = event.getMouseDownY();

  FilterParameters filterParameters(uiState.selectedFilterID,
                                    pluginProcessor.getAPVTS());

  filterParameters.isActive->beginChangeGesture();
  filterParameters.isActive->setValueNotifyingHost(1.0f);
  filterParameters.isActive->endChangeGesture();

  filterParameters.frequency->beginChangeGesture();
  filterParameters.frequency->setValueNotifyingHost(freqToNorm(xToFreq(x)));
  filterParameters.frequency->endChangeGesture();

  filterParameters.gain->beginChangeGesture();
  filterParameters.gain->setValueNotifyingHost(dBToNorm(yToDb(y)));
  filterParameters.gain->endChangeGesture();

  filterButtons[uiState.selectedFilterID - 1]->setCentrePosition(
      event.getMouseDownPosition());
  filterButtons[uiState.selectedFilterID - 1]->setVisible(true);

  update();
}

void Equalizer::update() {
  filterPanel.update();
  for (const auto &[filterID, isUsed] : uiState.usedFilterIDs) {
    filterFreqResponses[filterID - 1]->setVisible(isUsed);
    filterButtons[filterID - 1]->setVisible(isUsed);
  }
}
