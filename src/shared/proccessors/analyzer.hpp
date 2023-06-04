#pragma once

#include <functional>
#include <memory>
#include <valarray>
#include <vector>

#include "../dsp/dsp.hpp"
#include "base_processor.hpp"

class AnalyzerProcessor : public BaseProcessor {
public:
  AnalyzerProcessor();

  void prepareToPlay(double, int) override;
  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  void changeFFTOrder(int);
  void changeWindowKind(dsp::WindowKind);

  int getFFTSize() const { return fftSize; }

  void updateAmplitudes();
  const std::valarray<juce::LinearSmoothedValue<float>> &getAmplitudes() const {
    return amplitudes;
  }

private:
  void update();

  void pushNextSampleToAudioBlock(float);
  void moveAudioBlockSamples();

  int fftOrder = 12;
  int fftSize = 1 << 12;

  int audioBlockIndex = 0;
  std::vector<float> audioBlock;
  bool nextBlockReady = false;

  std::valarray<std::complex<float>> phasors;
  dsp::WindowKind windowKind = dsp::WindowKind::Nuttall;
  std::unique_ptr<dsp::Window> window;

  const float rampLengthAmplitudes = 0.5f;
  std::valarray<juce::LinearSmoothedValue<float>> amplitudes;
};