#pragma once

#include "shared.hpp"
#include "ui/ui.hpp"

#include <memory>
#include <vector>

class Equalizer : public LayoutComponent {
public:
  Equalizer(PluginProcessor &);

  void resized() override;

private:
  GridComponent grid;
  AnalyzerComponent analyzer;
  FrequencyResponseComponent freqResponse;
  std::vector<std::unique_ptr<FilterComponent>> filters;
};