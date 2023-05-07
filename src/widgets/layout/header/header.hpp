#pragma once

#include "shared.hpp"

#include "ui/ui.hpp"

class Header : public LayoutComponent {
public:
  static inline const float containerWidth = 1200.0f;
  static inline const float logoWidth = 140.0f;

public:
  Header();

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  Logo logo;
  RightButtonsStack rightButtonsStack;
};