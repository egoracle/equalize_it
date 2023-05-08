#pragma once

#include "shared.hpp"

#include "ui/ui.hpp"

class Equalizer : public LayoutComponent {
public:
  Equalizer(PluginProcessor &);

  void resized() override;

private:
  GridComponent grid;
  AnalyzerComponent analyzer;
  FrequencyResponseComponent freqResponse;
};