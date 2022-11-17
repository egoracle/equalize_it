#include "editor.hpp"

AppEditor::AppEditor(AppProcessor &audioProcessor)
    : juce::AudioProcessorEditor(&audioProcessor),
      audioProcessor(audioProcessor),
      equalizerEditor(audioProcessor.getEqualizerProcessor()) {
  setResizable(true, true);
  setSize(1280, 720);

  addAndMakeVisible(header);
  addAndMakeVisible(equalizerEditor);

  grid.templateRows = {Track(Px(header.minHeight)), Track(Px(30)), Track(Fr(1)),
                       Track(Px(30)), Track(Px(header.minHeight))};
  grid.templateColumns = {Track(Fr(1))};
  grid.items = {juce::GridItem(header), juce::GridItem(),
                juce::GridItem(equalizerEditor), juce::GridItem(),
                juce::GridItem()};

  resized();
}

AppEditor::~AppEditor() {}

void AppEditor::paint(juce::Graphics &g) { g.fillAll(juce::Colours::white); }

void AppEditor::resized() { grid.performLayout(getLocalBounds()); }