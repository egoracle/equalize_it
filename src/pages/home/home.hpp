#pragma once

#include "shared.hpp"
#include "widgets.hpp"

#include "equalizer/equalizer.hpp"

class HomePage : public LayoutComponent {
public:
  enum { headerHeight = 40 };

public:
  HomePage();

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  Header header;
  Equalizer equalizer;
};