#pragma once

#include "plot/plot.hpp"
#include "processor.hpp"
#include "shared.hpp"

class AnalyzerEditor : public shared::lib::BaseComponent {
public:
  class Container : public shared::lib::BaseComponent {
  public:
    Container(AnalyzerProcessor &audioProcessor);

  private:
    AnalyzerProcessor &audioProcessor;

    AnalyzerPlot analyzerPlot;
  };

public:
  enum { containerWidth = 1200 };

  AnalyzerEditor(AnalyzerProcessor &audioProcessor);

  void resized() override;

private:
  AnalyzerProcessor &audioProcessor;

  Container container;
};