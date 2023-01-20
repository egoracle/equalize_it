#pragma once

#include "shared.hpp"

#include <juce_audio_utils/juce_audio_utils.h>

class SpectrumPlot : public juce::AudioAppComponent, private juce::Timer {
public:
  enum { fftSize = 16384, hopSize = 2048 };

  SpectrumPlot(shared::lib::BaseProcessor &audioProcessor);
  ~SpectrumPlot() override;

  void prepareToPlay(int, double) override {}
  void releaseResources() override {}

  void
  getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
  void pushNextSampleIntoFifo(float sample) noexcept;
  void timerCallback() override;

  void update();
  void drawFrame(juce::Graphics &g);

  void paint(juce::Graphics &g) override;
  void resized() override {}

private:
  shared::lib::BaseProcessor &audioProcessor;

  shared::core::audio::NuttallWindow window;
  shared::core::audio::Spectrogram spectrogram;

  int fifoIndex = 0;
  float fifo[fftSize];
  bool nextFFTBlockReady = false;
  float amplitudes[fftSize];

  float cacheDecay = 0.1f;
  std::vector<float> cache;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumPlot)
};