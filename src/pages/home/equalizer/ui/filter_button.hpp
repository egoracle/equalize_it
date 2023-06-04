#pragma once

#include "shared.hpp"

#include <functional>
#include <vector>

class FilterButton : public juce::ShapeButton, private juce::Timer {
public:
  FilterButton(int, PluginProcessor &, std::function<void()>);

  void mouseDown(const juce::MouseEvent &event) override;
  void mouseDrag(const juce::MouseEvent &event) override;

  void timerCallback() override;

private:
  const int buttonSize = 20;

  int filterID;
  PluginProcessor &pluginProcessor;
  std::function<void()> updateEqualizerCallback;

  FilterParameters filterParameters;
  PluginProcessor::UiState &uiState;

  juce::ComponentDragger dragger;
  juce::ComponentBoundsConstrainer constrainer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterButton)
};