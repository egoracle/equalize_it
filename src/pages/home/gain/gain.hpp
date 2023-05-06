#pragma once

#include "shared.hpp"
#include "ui/level_meter.hpp"

class Gain : public LayoutComponent {
public:
  class LevelMeters : public LayoutComponent {
  public:
    LevelMeters(PluginProcessor &);

    void resized() override;

  private:
    LevelMeter levelMeterLeft;
    LevelMeter levelMeterRight;
  };

public:
  Gain(PluginProcessor &);

  void resized() override;

private:
  juce::Slider slider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      sliderAttachment;

  LevelMeters levelMeters;
};