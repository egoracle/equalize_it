#pragma once

#include "../../shared/shared.hpp"
#include "plot/plot.hpp"
#include "processor.hpp"

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