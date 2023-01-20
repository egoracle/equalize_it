#include "spectrum.hpp"

#include <cmath>

SpectrumPlot::SpectrumPlot(shared::lib::BaseProcessor &audioProcessor)
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
  g.setColour(shared::ui::Colours::primary);
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

    for (int i = 0; i < fftSize - hopSize; i++) {
      fifo[i] = fifo[hopSize + i - 1];
    }

    fifoIndex = fftSize - hopSize;
  }

  fifo[fifoIndex] = sample;
  fifoIndex++;
}

void SpectrumPlot::update() {
  spectrogram.update(fifo);
  spectrogram.unloadAmplitudes(amplitudes);

  auto mindB = -100.0f;
  auto maxdB = 0.0f;

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

  std::vector<float> X, Y;

  X.push_back(0);
  Y.push_back(juce::jmap(amplitudes[0], 0.0f, 1.0f, (float)height, 0.0f));

  for (int i = 1; i < fftSize / 2; i++) {
    float currFreq = i * sr / fftSize;

    auto currNorm = juce::mapFromLog10(currFreq, 20.f, 20000.f);

    float currX = (float)width * currNorm;

    X.push_back(currX);
  }

  for (int i = 1; i < fftSize / 2; i++) {
    Y.push_back(juce::jmap(amplitudes[i], 0.0f, 1.0f, (float)height, 0.0f));
  }

  std::vector<float> optimizedX;
  for (int i = 0; i < width; i++) {
    optimizedX.push_back(i);
  }

  auto optimizedY =
      shared::core::graphics::averagePoints(X, Y, optimizedX, height);

  juce::Path path;

  if (cache.size() == 0) {
    path.startNewSubPath(optimizedX[0], optimizedY[0]);
    cache.push_back(optimizedY[0]);

    for (int i = 1; i < optimizedX.size(); i++) {
      path.lineTo(optimizedX[i], optimizedY[i]);
      cache.push_back(optimizedY[i]);
    }

  } else {
    float y = (1 - cacheDecay) * optimizedY[0] + cacheDecay * cache[0];
    path.startNewSubPath(optimizedX[0], y);
    cache[0] = y;

    for (int i = 1; i < optimizedX.size(); i++) {
      float y = (1 - cacheDecay) * optimizedY[i] + cacheDecay * cache[i];
      cache[i] = y;
      path.lineTo(optimizedX[i], y);
    }
  }

  g.strokePath(
      path, juce::PathStrokeType(2.f, juce::PathStrokeType::JointStyle::curved,
                                 juce::PathStrokeType::EndCapStyle::rounded));

  g.setGradientFill(juce::ColourGradient(
      shared::ui::Colours::primary, width / 2, 0,
      juce::Colours::transparentWhite, width / 2, height, false));
  g.fillPath(path);
}