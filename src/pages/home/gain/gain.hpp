#pragma once

#include "shared.hpp"
#include "ui/level_meter.hpp"

class Gain : public LayoutComponent {
public:
  Gain(PluginProcessor &);

  void resized() override;

private:
  juce::Slider slider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      sliderAttachment;
  
  LevelMeterComponent levelMeter;
};