#include "editor.hpp"

AppEditor::AppEditor(AppProcessor &p)
    : juce::AudioProcessorEditor(&p), appProcessor(p) {
  setSize(defaultWidth, defaultHeight);
  setResizable(true, true);

  const auto desktopArea =
      juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;

  setResizeLimits(minWidth, minHeight, desktopArea.getWidth(),
                  desktopArea.getHeight());

  addAndMakeVisible(homePage);

  resized();
}

AppEditor::~AppEditor() {}

void AppEditor::paint(juce::Graphics &) {}

void AppEditor::resized() { homePage.setBounds(getLocalBounds()); }