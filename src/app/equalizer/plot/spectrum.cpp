#include "spectrum.hpp"

SpectrumPlot::SpectrumPlot(BaseProcessor &audioProcessor)
    : audioProcessor(audioProcessor), window(fftSize),
      spectrogram(fftSize, window) {
  setAudioChannels(2, 0);
  startTimerHz(30);
}

SpectrumPlot::~SpectrumPlot() { shutdownAudio(); }

void SpectrumPlot::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {
  if (bufferToFill.buffer->getNumChannels() > 0) {
    auto *channelData =
        bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

    for (auto i = 0; i < bufferToFill.numSamples; ++i)
      pushNextSampleIntoFifo(channelData[i]);
  }
}

void SpectrumPlot::paint(juce::Graphics &g) {
  g.setColour(juce::Colours::tomato);
  drawFrame(g);
}

void SpectrumPlot::timerCallback() {
  if (nextFFTBlockReady) {
    update();
    nextFFTBlockReady = false;
    repaint();
  }
}

void SpectrumPlot::pushNextSampleIntoFifo(float sample) noexcept {
  if (fifoIndex == fftSize) {
    if (!nextFFTBlockReady) {
      nextFFTBlockReady = true;
    }

    fifoIndex = 0;
  }

  fifo[fifoIndex++] = sample;
}

void SpectrumPlot::update() {
  spectrogram.update(fifo);
  spectrogram.unloadAmplitudes(amplitudes);

  auto mindB = -75.0f;
  auto maxdB = 25.0f;

  for (int i = 0; i < fftSize; i++) {
    auto level =
        juce::jmap(juce::jlimit(mindB, maxdB,
                                juce::Decibels::gainToDecibels(amplitudes[i])),
                   mindB, maxdB, 0.0f, 1.0f);

    amplitudes[i] = level;
  }
}

void SpectrumPlot::drawFrame(juce::Graphics &g) {
  auto sr = audioProcessor.getSampleRate();
  auto width = getWidth();
  auto height = getHeight();

  std::vector<double> X, Y;

  X.push_back(0);
  Y.push_back(juce::jmap(amplitudes[0], 0.0f, 1.0f, (float)height, 0.0f));

  for (int i = 1; i < fftSize / 2; i++) {
    float currFreq = i * sr / fftSize;

    auto currNorm = juce::mapFromLog10(currFreq, 10.f, 20001.f);

    float currX = (float)width * currNorm;

    X.push_back(currX);
  }

  for (int i = 1; i < fftSize / 2; i++) {
    Y.push_back(juce::jmap(amplitudes[i], 0.0f, 1.0f, (float)height, 0.0f));
  }

  juce::Path path;
  path.startNewSubPath(X[0], Y[0]);

  for (int i = 1; i < X.size(); i++) {
    path.lineTo(X[i], Y[i]);
  }

  g.strokePath(
      path, juce::PathStrokeType(2.f, juce::PathStrokeType::JointStyle::curved,
                                 juce::PathStrokeType::EndCapStyle::rounded));
}