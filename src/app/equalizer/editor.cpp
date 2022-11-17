#include "editor.hpp"

EqualizerEditor::Container::Container(EqualizerProcessor &audioProcessor)
    : audioProcessor(audioProcessor), equalizerPlot(audioProcessor) {
  addAndMakeVisible(equalizerPlot);

  grid.templateRows = {Track(Fr(1))};
  grid.templateColumns = {Track(Fr(1))};
  grid.items = {juce::GridItem(equalizerPlot)};

  resized();
}

EqualizerEditor::EqualizerEditor(EqualizerProcessor &audioProcessor)
    : audioProcessor(audioProcessor), container(audioProcessor) {
  addAndMakeVisible(container);

  grid.templateRows = {Track(Fr(1))};
  grid.items = {juce::GridItem(), juce::GridItem(container), juce::GridItem()};

  resized();
}

void EqualizerEditor::resized() {
  grid.templateColumns = {
      Track(Fr(1)), Track(Px(juce::jmin<float>(containerWidth, getWidth()))),
      Track(Fr(1))};

  grid.performLayout(getLocalBounds());
}