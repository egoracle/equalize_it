#pragma once

#include <functional>

#include "shared.hpp"

class FilterPanel : public LayoutComponent {
public:
  class ParametersWrapper : public LayoutComponent {
  public:
    ParametersWrapper(int, PluginProcessor &);

    void resized() override;

  private:
    int filterID;
    PluginProcessor &pluginProcessor;

    EqItSlider qualitySlider;
    std::unique_ptr<types::APVTS::SliderAttachment> qualitySliderAttachment;

    EqItSlider frequencySlider;
    std::unique_ptr<types::APVTS::SliderAttachment> frequencySliderAttachment;

    EqItSlider gainSlider;
    std::unique_ptr<types::APVTS::SliderAttachment> gainSliderAttachment;

    juce::ComboBox typeComboBox;
    std::unique_ptr<types::APVTS::ComboBoxAttachment> typeComboBoxAttachment;

    IconButton lowpassButton;
    IconButton peakButton;
    IconButton highpassButton;

    juce::ToggleButton activeFilterButton;
    std::unique_ptr<types::APVTS::ButtonAttachment>
        activeFilterButtonAttachment;
  };

  FilterPanel(PluginProcessor &, std::function<void()>);

  void paint(juce::Graphics &g) override;
  void resized() override;

  void mouseDown(const juce::MouseEvent &event) override {
    LayoutComponent::mouseDown(event);
    dragger.startDraggingComponent(this, event);
  }

  void mouseDrag(const juce::MouseEvent &event) override {
    LayoutComponent::mouseDrag(event);
    dragger.dragComponent(this, event, &constrainer);
  }

  void update();

private:
  juce::Path createPrevTrianglePath();
  juce::Path createNextTrianglePath();

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
  std::vector<std::unique_ptr<ParametersWrapper>> parameterWrappers;

  juce::ComponentDragger dragger;
  juce::ComponentBoundsConstrainer constrainer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterPanel)
};