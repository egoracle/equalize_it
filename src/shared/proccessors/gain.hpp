#pragma once

#include "../parameters/gain.hpp"
#include "../types.hpp"
#include "base_processor.hpp"

class GainProcessor : public BaseProcessor {
public:
  GainProcessor(types::APVTS &);

  void prepareToPlay(double, int) override;
  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  float getRmsValue(int);

private:
  void updateRmsValue(juce::AudioSampleBuffer &, int);
  void applyGain(juce::AudioSampleBuffer &);

  GainParameters params;
  float previousWetValueDb;

  juce::LinearSmoothedValue<float> rmsValueLeft, rmsValueRight;
};
