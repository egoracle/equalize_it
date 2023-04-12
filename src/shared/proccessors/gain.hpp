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

  void updateRmsValues(juce::AudioSampleBuffer &);
  void applyGain(juce::AudioSampleBuffer &);

private:
  GainParameters extractGainParameters(APVTS &);

  GainParameters params;
  float previousWetValueDb;
  float rmsValueLeft;
  float rmsValueRight;
};
