#include "editor.hpp"

AnalyzerEditor::Container::Container(AnalyzerProcessor &audioProcessor)
    : audioProcessor(audioProcessor), analyzerPlot(audioProcessor) {
  addAndMakeVisible(analyzerPlot);

  grid.templateRows = {Track(Fr(1))};
  grid.templateColumns = {Track(Fr(1))};
  grid.items = {juce::GridItem(analyzerPlot)};

  resized();
}

AnalyzerEditor::AnalyzerEditor(AnalyzerProcessor &audioProcessor)
    : audioProcessor(audioProcessor), container(audioProcessor) {
  addAndMakeVisible(container);

  grid.templateRows = {Track(Fr(1))};
  grid.items = {juce::GridItem(), juce::GridItem(container), juce::GridItem()};

  resized();
}

void AnalyzerEditor::resized() {
  grid.templateColumns = {
      Track(Fr(1)), Track(Px(juce::jmin<float>(containerWidth, getWidth()))),
      Track(Fr(1))};

  grid.performLayout(getLocalBounds());
}