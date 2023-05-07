#include "editor.hpp"

PluginEditor::PluginEditor(PluginProcessor &p)
    : juce::AudioProcessorEditor(&p), homePage(p) {
  setSize(defaultWidth, defaultHeight);
  setResizable(true, true);

  const auto desktopArea =
      juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;

  setResizeLimits(minWidth, minHeight, desktopArea.getWidth(),
                  desktopArea.getHeight());

  juce::LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);

  addAndMakeVisible(homePage);

  resized();
}

PluginEditor::~PluginEditor() {
  juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void PluginEditor::resized() { homePage.setBounds(getLocalBounds()); }