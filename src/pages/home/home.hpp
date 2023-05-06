#pragma once

#include "shared.hpp"
#include "widgets.hpp"

#include "equalizer/equalizer.hpp"
#include "gain/gain.hpp"

class HomePage : public LayoutComponent {
public:
  enum { headerHeight = 40 };

  class Body : public LayoutComponent {
  public:
    Body(PluginProcessor &);

    void resized() override;

  private:
    Equalizer equalizer;
    Gain gain;
  };

public:
  HomePage(PluginProcessor &);

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  Header header;
  Body body;
};