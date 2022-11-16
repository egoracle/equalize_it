#include "editor.hpp"

AppEditor::AppEditor(AppProcessor &audioProcessor)
    : BaseEditor(audioProcessor) {
  setResizable(true, true);
  setSize(1280, 720);

  addAndMakeVisible(header);

  grid.templateRows = {Track(Px(header.minHeight)), Track(Fr(1)),
                       Track(Px(header.minHeight))};
  grid.templateColumns = {Track(Fr(1))};
  grid.items = {juce::GridItem(header), juce::GridItem(), juce::GridItem()};

  grid.performLayout(getLocalBounds());
}

void AppEditor::paint(juce::Graphics &g) { g.fillAll(juce::Colours::white); }