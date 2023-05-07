#include "home.hpp"

HomePage::HomePage(PluginProcessor &pluginProcessor) : body(pluginProcessor) {
  addAndMakeVisible(body);
  addAndMakeVisible(header);

  resized();
}

void HomePage::paint(juce::Graphics &g) { g.fillAll(juce::Colours::white); }

void HomePage::resized() {
  layout.templateRows = {Track(Px(headerHeight)), Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1))};
  layout.items = {juce::GridItem(header), juce::GridItem(body)};

  LayoutComponent::resized();
}

HomePage::Body::Body(PluginProcessor &pluginProcessor)
    : equalizer(pluginProcessor), gain(pluginProcessor) {
  addAndMakeVisible(equalizer);
  addAndMakeVisible(gain);

  resized();
}

void HomePage::Body::resized() {
  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Px(15)), Track(Fr(1)), Track(Px(55)),
                            Track(Px(15))};
  layout.items = {juce::GridItem(), juce::GridItem(equalizer),
                  juce::GridItem(gain), juce::GridItem()};

  LayoutComponent::resized();
}