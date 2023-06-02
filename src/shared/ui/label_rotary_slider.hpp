#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LabelRotarySlider : public juce::Slider {
public:
  class LookAndFeel : public juce::LookAndFeel_V4 {
  public:
    void drawRotarySlider(juce::Graphics &, int, int, int, int, float, float,
                          float, juce::Slider &) override;
    juce::Label *createSliderTextBox(juce::Slider &) override;
  };

  LabelRotarySlider(juce::String);
  LabelRotarySlider();

  ~LabelRotarySlider();

  juce::String &getLabel() noexcept;

private:
  LookAndFeel lookAndFeel;
  juce::String label;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabelRotarySlider)
};