#include "level_meter.hpp"

LevelMeterComponent::LevelMeterComponent(PluginProcessor &pluginProcessor)
    : leftBar(pluginProcessor, 0), rightBar(pluginProcessor, 1) {
  addAndMakeVisible(leftBar);
  addAndMakeVisible(scale);
  addAndMakeVisible(rightBar);

  resized();
}

void LevelMeterComponent::resized() {
  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Px(10)), Track(Fr(1)), Track(Px(10))};
  layout.items = {juce::GridItem(leftBar), juce::GridItem(scale),
                  juce::GridItem(rightBar)};

  LayoutComponent::resized();
}