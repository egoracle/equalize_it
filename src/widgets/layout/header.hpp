#pragma once

#include "../../shared/shared.hpp"

class Header : public BaseComponent {
public:
  class Container : public BaseComponent {
  public:
    Container();
  };

public:
  enum { containerWidth = 1200, minHeight = 60 };

  Header();

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  Container container;
};