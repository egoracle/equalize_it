#include "header.hpp"

widgets::Header::Container::Container() {
  addAndMakeVisible(logo);

  logo.setText("Spectrum Analyzer", juce::dontSendNotification);
  logo.setFont(juce::Font(24.f));
  logo.setColour(juce::Label::textColourId, shared::ui::Colours::primary);
  logo.setJustificationType(juce::Justification::centred);

  grid.templateRows = {Track(Fr(1))};
  grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
  grid.items = {juce::GridItem(), juce::GridItem(logo), juce::GridItem()};

  resized();
}

widgets::Header::Header() {
  addAndMakeVisible(container);

  grid.templateRows = {Track(Fr(1))};
  grid.items = {juce::GridItem(), juce::GridItem(container), juce::GridItem()};

  resized();
}

void widgets::Header::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::white);
}

void widgets::Header::resized() {
  grid.templateColumns = {
      Track(Fr(1)), Track(Px(juce::jmin<float>(containerWidth, getWidth()))),
      Track(Fr(1))};

  grid.performLayout(getLocalBounds());
}