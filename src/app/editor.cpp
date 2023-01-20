#include "editor.hpp"

app::AppEditor::AppEditor(AppProcessor &audioProcessor)
    : juce::AudioProcessorEditor(&audioProcessor),
      audioProcessor(audioProcessor),
      analyzerEditor(audioProcessor.getAnalyzerProcessor()) {
  juce::LookAndFeel::setDefaultLookAndFeel(&fontLookAndFeel);

  setResizable(true, true);
  setSize(1280, 720);

  addAndMakeVisible(header);
  addAndMakeVisible(analyzerEditor);

  grid.templateRows = {Track(Px(header.minHeight)), Track(Px(30)), Track(Fr(1)),
                       Track(Px(header.minHeight))};
  grid.templateColumns = {Track(Fr(1))};
  grid.items = {juce::GridItem(header), juce::GridItem(),
                juce::GridItem(analyzerEditor), juce::GridItem()};

  resized();
}

app::AppEditor::~AppEditor() {
  juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void app::AppEditor::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::white);
}

void app::AppEditor::resized() { grid.performLayout(getLocalBounds()); }