#pragma once

#include "shared.hpp"

#include <juce_audio_utils/juce_audio_utils.h>

class SpectrumPlot : public juce::AudioAppComponent, private juce::Timer {
public:
  enum { fftSize = 16384, hopSize = 512 };

  SpectrumPlot(BaseProcessor &audioProcessor);
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
  BaseProcessor &audioProcessor;

  HammingWindow window;
  Spectrogram spectrogram;

  int fifoIndex = 0;
  float fifo[fftSize];
  bool nextFFTBlockReady = false;
  float prevAmplitudes[fftSize];
  float amplitudes[fftSize];
  float avgAmplitudes[fftSize];

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumPlot)
};