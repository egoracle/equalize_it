#define _USE_MATH_DEFINES

#include "analyzer.hpp"

#include <spline.h>
#include <vector>

AnalyzerComponent::AnalyzerComponent(PluginProcessor &p) : pluginProcessor(p) {
  startTimerHz(30);
}

void AnalyzerComponent::paint(juce::Graphics &g) {
  auto analyzerProcessor = pluginProcessor.getAnalyzerProcessor();

  if (!analyzerProcessor) {
    return;
  }

  float sr = float(analyzerProcessor->getSampleRate());

  auto preSpectrum = calculatePreSpectrum();
  auto postSpectrum = calculatePostSpectrum(preSpectrum);

  float w = float(getWidth()) - 1;
  float h = float(getHeight()) - 1;

  auto xToFreq = math::invLogMapping(0.0f, w, 5.0f, 50000.0f);
  auto dBToY = math::segmentMapping(-100.0f, 10.f, -h, 0.0f);

  juce::Path preSpectrumPath;
  for (int i = 1; i <= int(w); ++i) {
    float x = float(i);
    float freq = xToFreq(x);

    if (freq > sr / 2) {
      break;
    }

    float y = -dBToY(preSpectrum(freq));

    if (i == 1) {
      preSpectrumPath.startNewSubPath(x, y);
    } else {
      preSpectrumPath.lineTo(x, y);
    }
  }

  g.setColour(juce::Colours::green);
  g.strokePath(
      preSpectrumPath,
      juce::PathStrokeType(2.f, juce::PathStrokeType::JointStyle::curved,
                           juce::PathStrokeType::EndCapStyle::rounded));

  juce::Path postSpectrumPath;
  for (int i = 1; i <= w; ++i) {
    float x = float(i);
    float freq = xToFreq(x);

    if (freq > sr / 2) {
      break;
    }

    float y = -dBToY(postSpectrum(freq));

    if (i == 1) {
      postSpectrumPath.startNewSubPath(x, y);
    } else {
      postSpectrumPath.lineTo(x, y);
    }
  }

  g.setColour(juce::Colours::red);
  g.strokePath(
      postSpectrumPath,
      juce::PathStrokeType(2.f, juce::PathStrokeType::JointStyle::curved,
                           juce::PathStrokeType::EndCapStyle::rounded));
}

void AnalyzerComponent::timerCallback() {
  auto analyzerProcessor = pluginProcessor.getAnalyzerProcessor();
  if (analyzerProcessor) {
    analyzerProcessor->updateAmplitudes();
  }

  repaint();
}

std::function<float(float)> AnalyzerComponent::calculatePreSpectrum() {
  auto analyzerProcessor = pluginProcessor.getAnalyzerProcessor();
  jassert(analyzerProcessor);

  float w = float(getWidth()) - 1;
  float h = float(getHeight()) - 1;

  float sr = float(analyzerProcessor->getSampleRate());
  int fftSize = analyzerProcessor->getFFTSize();
  auto amplitudes = analyzerProcessor->getAmplitudes();

  auto binToFreq = dsp::binToFrequencyMapping(fftSize, sr);
  auto dBToY = math::segmentMapping(-100.0f, 10.0f, -h, 0.0f);

  std::vector<double> freqs, dBs;
  for (int i = 1; i < amplitudes.size(); ++i) {
    float freq = binToFreq(i);

    freqs.push_back(freq);
    dBs.push_back(amplitudes[i].getCurrentValue());
  }
  tk::spline cubicSpline(freqs, dBs);

  return [=](float freq) { return cubicSpline(freq); };
}

std::function<float(float)> AnalyzerComponent::calculatePostSpectrum(
    std::function<float(float)> &preSpectrum) {
  auto equalizerProcessor = pluginProcessor.getEqualizerProcessor();
  jassert(equalizerProcessor);

  auto freqResponse = equalizerProcessor->getFrequencyResponse();

  return [=](float freq) {
    float preDB = preSpectrum(freq);
    if (preDB <= -100.0f) {
      return preDB;
    }
    return preDB + freqResponse(freq);
  };
}