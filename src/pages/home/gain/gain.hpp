#pragma once

#include "shared.hpp"
#include "ui/level_meter.hpp"

class Gain : public LayoutComponent {
public:
  Gain(PluginProcessor &);

  void resized() override;

private:
  LabelRotarySlider slider;
  std::unique_ptr<types::APVTS::SliderAttachment> sliderAttachment;

  LevelMeterComponent levelMeter;
};