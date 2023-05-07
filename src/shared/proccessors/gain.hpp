#pragma once

#include "lib/lib.hpp"

struct GainParameters {
  juce::AudioParameterFloat *wet;

  static void addToLayout(APVTS::ParameterLayout &);

  static inline juce::String getWetID() noexcept;
  static inline juce::String getWetName() noexcept;

  float getWetDbValue();
};

class GainProcessor : public BaseProcessor {
public:
  GainProcessor(APVTS &);

  void prepareToPlay(double, int) override;
  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  float getRmsValue(int);

private:
  GainParameters extractGainParameters(APVTS &);

  void updateRmsValue(juce::AudioSampleBuffer &, int);
  void applyGain(juce::AudioSampleBuffer &);

  GainParameters params;
  float previousWetValueDb;

  juce::LinearSmoothedValue<float> rmsValueLeft, rmsValueRight;
};
