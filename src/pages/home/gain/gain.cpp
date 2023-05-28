#include "gain.hpp"

Gain::Gain(PluginProcessor &pluginProcessor)
    : slider(""), levelMeter(pluginProcessor) {
  addAndMakeVisible(levelMeter);

  slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
  slider.setRange(0.0, 1.0, 0.01);
  slider.setTextValueSuffix(" dB");
  addAndMakeVisible(slider);

  sliderAttachment = std::make_unique<types::APVTS::SliderAttachment>(
      pluginProcessor.getAPVTS(), GainParameters::getWetID(), slider);

  resized();
}

void Gain::resized() {
  layout.templateRows = {Track(Fr(1)), Track(Px(75))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(levelMeter), juce::GridItem(slider)};

  LayoutComponent::resized();
}
