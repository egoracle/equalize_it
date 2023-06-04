#include "filter_button.hpp"

#include "constants.hpp"

FilterButton::FilterButton(int id, PluginProcessor &p,
                           std::function<void()> uec)
    : ShapeButton(juce::String::formatted("filter_button_%d", id),
                  colours::getFilterColour(id),
                  colours::getFilterColour(id).darker(0.25),
                  colours::getFilterColour(id)),
      filterID(id), pluginProcessor(p), updateEqualizerCallback(uec),
      filterParameters(FilterParameters(id, p.getAPVTS())),
      uiState(p.getUiState()) {
  startTimerHz(24);

  setShape(paths::createCircle(), true, true, false);
  setOutline(colours::getFilterColour(id).brighter(1.25), 3.0f);

  setSize(buttonSize, buttonSize);

  constrainer.setMinimumOnscreenAmounts(buttonSize, buttonSize, buttonSize,
                                        buttonSize);
}

void FilterButton::mouseDown(const juce::MouseEvent &event) {
  ShapeButton::mouseDown(event);

  if (event.mods.isRightButtonDown()) {
    uiState.removeFilter(filterID);

    filterParameters.isActive->beginChangeGesture();
    filterParameters.isActive->setValueNotifyingHost(0.0f);
    filterParameters.isActive->endChangeGesture();

    setVisible(false);
    updateEqualizerCallback();

    return;
  }

  uiState.selectedFilterID = filterID;
  updateEqualizerCallback();

  return;

  dragger.startDraggingComponent(this, event);
}

void FilterButton::mouseDrag(const juce::MouseEvent &event) {
  ShapeButton::mouseDrag(event);

  return;

  dragger.dragComponent(this, event, &constrainer);
}

void FilterButton::timerCallback() {
  const auto parent = getParentComponent();

  const float xMin = 0.0f;
  const float xMax = static_cast<float>(parent->getWidth());
  const float yMax = static_cast<float>(parent->getHeight());

  const auto freqToX = math::logMapping(constants::GRID_MIN_FREQ,
                                        constants::GRID_MAX_FREQ, xMin, xMax);
  const auto dBToY = math::segmentMapping(constants::GRID_MIN_DB,
                                          constants::GRID_MAX_DB, yMax, 0.0f);

  const int x = static_cast<int>(freqToX(filterParameters.getFrequencyValue()));
  const int y = static_cast<int>(dBToY(filterParameters.gain->get()));
  setCentrePosition(x, y);
}