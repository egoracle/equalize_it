#pragma once

#include "lib/lib.hpp"

class GainProcessor : public BaseProcessor {
public:
  GainProcessor(juce::AudioProcessorValueTreeState &);

  void prepareToPlay(double, int) override;
  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  float getRmsValue(int);

  void updateRmsValues(juce::AudioSampleBuffer &);
  void applyGain(juce::AudioSampleBuffer &);

private:
  juce::AudioProcessorValueTreeState &apvts;

  float previousGainValue;

  float rmsValueLeft;
  float rmsValueRight;
};