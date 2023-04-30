#pragma once

#include "../dsp/dsp.hpp"
#include "lib/lib.hpp"

class AnalyzerProcessor : public BaseProcessor {
public:
  enum { FFT_SIZE = 8192, HOP_SIZE = 256 };

  AnalyzerProcessor();

  void prepareToPlay(double, int) override;
  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  void updateAmplitudes();
  std::valarray<juce::LinearSmoothedValue<float>> &getAmplitudes();

private:
  void pushNextSampleToAudioBlock(float);

  int audioBlockIndex = 0;
  std::valarray<std::complex<float>> audioBlock;
  bool nextBlockReady = false;

  std::valarray<std::complex<float>> window;

  std::valarray<juce::LinearSmoothedValue<float>> amplitudes;
};