#pragma once

#include "shared.hpp"

class Logo : public LayoutComponent {
public:
  Logo();

  void paint(juce::Graphics &g) override;

private:
  std::unique_ptr<juce::Drawable> drawable;
};