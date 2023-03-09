#pragma once

#include "shared.hpp"

#include "ui/ui.hpp"

class Equalizer : public LayoutComponent {
public:
  Equalizer();

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  Grid grid;
};