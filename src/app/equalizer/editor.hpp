#pragma once

#include "plot/plot.hpp"
#include "processor.hpp"
#include "shared.hpp"

class EqualizerEditor : public BaseComponent {
public:
  class Container : public BaseComponent {
  public:
    Container(EqualizerProcessor &audioProcessor);

  private:
    EqualizerProcessor &audioProcessor;

    EqualizerPlot equalizerPlot;
  };

public:
  enum { containerWidth = 1200 };

  EqualizerEditor(EqualizerProcessor &audioProcessor);

  void resized() override;

private:
  EqualizerProcessor &audioProcessor;

  Container container;
};