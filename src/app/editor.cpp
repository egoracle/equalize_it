#include "editor.hpp"

AppEditor::AppEditor(AppProcessor &audioProcessor)
    : AudioProcessorEditor(&audioProcessor), audioProcessor(audioProcessor) {
  setResizable(true, true);
  setSize(1280, 720);
}

AppEditor::~AppEditor() {}

void AppEditor::paint(juce::Graphics &g) {}

void AppEditor::resized() {}