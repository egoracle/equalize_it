#include "right_buttons_stack.hpp"

#include <BinaryData.h>

RightButtonsStack::FullScreenButton::FullScreenButton()
    : IconButton("fullscreen", BinaryData::fullscreen_svg) {}

RightButtonsStack::SettingsButton::SettingsButton()
    : IconButton("settings", BinaryData::settings_svg) {}

RightButtonsStack::RightButtonsStack() : LayoutComponent() {
  addAndMakeVisible(fullScreenButton);
  addAndMakeVisible(settingsButton);

  resized();
}

void RightButtonsStack::resized() {
  float buttomMarginPx = float(getHeight() - buttonSize) / 2;
  const auto buttonMargin =
      juce::GridItem::Margin(buttomMarginPx, 0, buttomMarginPx, 0);

  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Px(buttonSize)), Track(Px(buttonSize)),
                            Track(Px(buttonSize))};
  layout.alignItems = juce::Grid::AlignItems::center;
  layout.justifyContent = juce::Grid::JustifyContent::end;
  layout.columnGap = Px(30);
  layout.items = {juce::GridItem(settingsButton).withMargin(buttonMargin),
                  juce::GridItem(fullScreenButton).withMargin(buttonMargin)};

  LayoutComponent::resized();
}
