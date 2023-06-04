#include "editor.hpp"

PluginEditor::PluginEditor(PluginProcessor &p)
    : juce::AudioProcessorEditor(&p), homePage(p) {
  juce::LookAndFeel_V4::setDefaultLookAndFeel(&lookAndFeel);

  const auto desktopArea =
      juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
  setSize(defaultWidth, defaultHeight);
  setResizable(true, true);
  setResizeLimits(minWidth, minHeight, desktopArea.getWidth(),
                  desktopArea.getHeight());

  addAndMakeVisible(homePage);

  resized();
}

PluginEditor::~PluginEditor() {
  juce::LookAndFeel_V4::setDefaultLookAndFeel(nullptr);
}

void PluginEditor::resized() { homePage.setBounds(getLocalBounds()); }