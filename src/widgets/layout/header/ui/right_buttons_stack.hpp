#pragma once

#include "shared.hpp"

class RightButtonsStack : public LayoutComponent {
public:
  enum { buttonSize = 20 };

public:
  class FullScreenButton : public IconButton {
  public:
    FullScreenButton();
  };

  class SettingsButton : public IconButton {
  public:
    SettingsButton();
  };

public:
  RightButtonsStack();

  void resized() override;

private:
  FullScreenButton fullScreenButton;
  SettingsButton settingsButton;
};