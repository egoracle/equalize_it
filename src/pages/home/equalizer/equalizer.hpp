#pragma once

#include "shared.hpp"
#include "ui/ui.hpp"

#include <memory>
#include <vector>

class Equalizer : public LayoutComponent {
public:
  Equalizer(PluginProcessor &);

  void mouseDown(const juce::MouseEvent &event) override {
    LayoutComponent::mouseDown(event);

    if (event.eventComponent == &freqResponse) {
      uiState.selectedFilterID = -1;
      update();
    }
  }

  void mouseDoubleClick(const juce::MouseEvent &event) override {
    LayoutComponent::mouseDoubleClick(event);

    if (event.eventComponent == &freqResponse) {
      const bool wasAdded = uiState.addFilter();
      if (wasAdded) {
        const float xMin = 0.0f;
        const float xMax = static_cast<float>(event.eventComponent->getWidth());
        const float yMax =
            static_cast<float>(event.eventComponent->getHeight());

        const auto xToFreq = math::invLogMapping(
            xMin, xMax, constants::GRID_MIN_FREQ, constants::GRID_MAX_FREQ);
        const auto freqToNorm = math::segmentMapping(20, 20000, 0, 1);
        const auto yToDb = math::segmentMapping(
            yMax, 0.0f, constants::GRID_MIN_DB, constants::GRID_MAX_DB);
        const auto dBToNorm = math::segmentMapping(-12, 12, 0, 1);

        const int x = event.getMouseDownX();
        const int y = event.getMouseDownY();

        FilterParameters filterParameters(uiState.selectedFilterID,
                                          pluginProcessor.getAPVTS());

        filterParameters.isActive->beginChangeGesture();
        filterParameters.isActive->setValueNotifyingHost(1.0f);
        filterParameters.isActive->endChangeGesture();

        filterParameters.frequency->beginChangeGesture();
        filterParameters.frequency->setValueNotifyingHost(
            freqToNorm(xToFreq(x)));
        filterParameters.frequency->endChangeGesture();

        filterParameters.gain->beginChangeGesture();
        filterParameters.gain->setValueNotifyingHost(dBToNorm(yToDb(y)));
        filterParameters.gain->endChangeGesture();

        filterButtons[uiState.selectedFilterID - 1]->setCentrePosition(
            event.getMouseDownPosition());
        filterButtons[uiState.selectedFilterID - 1]->setVisible(true);

        update();
      }
    }
  }

  void resized() override;

  void update();

private:
  PluginProcessor &pluginProcessor;
  PluginProcessor::UiState &uiState;

  GridComponent grid;
  AnalyzerComponent analyzer;
  std::vector<std::unique_ptr<FilterFrequencyResponse>> filterFreqResponses;
  EqualizerFrequencyResponse freqResponse;
  FilterPanel filterPanel;
  std::vector<std::unique_ptr<FilterButton>> filterButtons;
};