#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "shared.hpp"

namespace widgets {

class Header : public shared::lib::BaseComponent {
public:
  class Container : public shared::lib::BaseComponent {
  public:
    Container();

  private:
    juce::Label logo;
  };

public:
  enum { containerWidth = 1200, minHeight = 60 };

  Header();

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  Container container;
};

} // namespace widgets