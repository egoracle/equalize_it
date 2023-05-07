#include "gain.hpp"

Gain::Gain(PluginProcessor &pluginProcessor) : levelMeter(pluginProcessor) {
  addAndMakeVisible(levelMeter);

  slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
  slider.setRange(0.0, 1.0, 0.01);
  addAndMakeVisible(slider);

  sliderAttachment = std::make_unique<APVTS::SliderAttachment>(
      pluginProcessor.getAPVTS(), GainParameters::getWetID(), slider);

  resized();
}

void Gain::resized() {
  layout.templateRows = {Track(Fr(1)), Track(Px(75))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(levelMeter), juce::GridItem(slider)};

  LayoutComponent::resized();
}
