#include "gain.hpp"

Gain::Gain(PluginProcessor &pluginProcessor) : levelMeters(pluginProcessor) {
  slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
  slider.setRange(0.0, 1.0, 0.01);
  addAndMakeVisible(slider);
  addAndMakeVisible(levelMeters);

  sliderAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          pluginProcessor.getAPVTS(), "gain", slider);

  resized();
}

void Gain::resized() {
  layout.templateRows = {Track(Fr(1)), Track(Px(75))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(levelMeters), juce::GridItem(slider)};

  LayoutComponent::resized();
}

Gain::LevelMeters::LevelMeters(PluginProcessor &pluginProcessor)
    : levelMeterLeft(0, pluginProcessor), levelMeterRight(1, pluginProcessor) {
  addAndMakeVisible(levelMeterLeft);
  addAndMakeVisible(levelMeterRight);

  resized();
}

void Gain::LevelMeters::resized() {
  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1)), Track(Px(25)), Track(Fr(1))};
  layout.items = {juce::GridItem(levelMeterLeft), juce::GridItem(),
                  juce::GridItem(levelMeterRight)};

  LayoutComponent::resized();
}
