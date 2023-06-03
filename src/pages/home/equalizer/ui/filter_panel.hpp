#pragma once

#include <functional>

#include "shared.hpp"

class FilterPanel : public LayoutComponent {
public:
  class Wrapper : public LayoutComponent {
  public:
    Wrapper(int, PluginProcessor &);

    void resized() override;

  private:
    int filterID;
    PluginProcessor &pluginProcessor;

    LabelRotarySlider qualitySlider;
    std::unique_ptr<types::APVTS::SliderAttachment> qualitySliderAttachment;

    LabelRotarySlider frequencySlider;
    std::unique_ptr<types::APVTS::SliderAttachment> frequencySliderAttachment;

    LabelRotarySlider gainSlider;
    std::unique_ptr<types::APVTS::SliderAttachment> gainSliderAttachment;

    IconButton lowpassButton;
    IconButton peakButton;
    IconButton highpassButton;
  };

  FilterPanel(PluginProcessor &, std::function<void()>);

  void paint(juce::Graphics &g) override;
  void resized() override;

  void mouseDown(const juce::MouseEvent &event) override;
  void mouseDrag(const juce::MouseEvent &event) override;

  void update();

private:
  PluginProcessor &pluginProcessor;
  std::function<void()> updateEqualizerCallback;

  PluginProcessor::UiState &uiState;

  const float cornerSize = 10.0f;
  const float lineThickness = 3.0f;

  const float paddingTop = 10.0f;
  const float paddingBottom = 10.0f;
  const float paddingLeft = 10.0f;
  const float paddingRight = 10.0f;

  const float headerHeight = 30.0f;
  const float buttonSize = 15.0f;
  const float labelSize = 100.0f;

  juce::ShapeButton prevFilterButton;
  juce::ShapeButton nextFilterButton;
  juce::Label filterLabel;
  std::vector<std::unique_ptr<Wrapper>> wrappers;

  juce::ComponentDragger dragger;
  juce::ComponentBoundsConstrainer constrainer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterPanel)
};