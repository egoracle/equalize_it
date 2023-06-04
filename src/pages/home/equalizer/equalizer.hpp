#pragma once

#include "shared.hpp"
#include "ui/ui.hpp"

#include <memory>
#include <vector>

class Equalizer : public LayoutComponent {
public:
  Equalizer(PluginProcessor &);

  void resized() override;

  void mouseDown(const juce::MouseEvent &event) override;
  void mouseDoubleClick(const juce::MouseEvent &event) override;

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