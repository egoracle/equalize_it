#define _USE_MATH_DEFINES

#include "analyzer.hpp"
#include "constants.hpp"

#include <spline.h>
#include <vector>

AnalyzerComponent::AnalyzerComponent(PluginProcessor &p) : pluginProcessor(p) {
  startTimerHz(24);
}

void AnalyzerComponent::paint(juce::Graphics &g) {
  auto equalizerProcessor = pluginProcessor.getEqualizerProcessor();
  auto analyzerProcessor = pluginProcessor.getAnalyzerProcessor();

  if (!equalizerProcessor || !analyzerProcessor) {
    return;
  }

  const float xMin = 0.0f;
  const float xMax = static_cast<float>(getWidth());
  const float yMin = 0.0f;
  const float yMax = static_cast<float>(getHeight());

  const float sampleRate =
      static_cast<float>(analyzerProcessor->getSampleRate());
  const float halfSampleRate = sampleRate * 0.5f;
  const int fftSize = analyzerProcessor->getFFTSize();

  const auto xToFreq = math::invLogMapping(xMin, xMax, constants::GRID_MIN_FREQ,
                                           constants::GRID_MAX_FREQ);
  const auto dBToY = math::segmentMapping(
      constants::MINUS_INFINITY_DB, constants::ANALYZER_MAX_DB, yMax, 0.0f);

  std::function<float(float)> preSpectrum, postSpectrum;
  {
    const auto amplitudes = analyzerProcessor->getAmplitudes();
    const auto binToFreq = dsp::binToFrequencyMapping(fftSize, sampleRate);

    std::vector<double> freqs, dBs;
    for (std::size_t i = 1; i < amplitudes.size(); ++i) {
      const float freq = binToFreq(static_cast<int>(i));
      freqs.push_back(freq);
      dBs.push_back(amplitudes[i].getCurrentValue());
    }

    tk::spline preSpectrumSpline(freqs, dBs);
    preSpectrum = [preSpectrumSpline](float freq) {
      return preSpectrumSpline(freq);
    };

    const auto freqResponse = equalizerProcessor->getFrequencyResponse();
    postSpectrum = [freqResponse, &preSpectrum](float freq) {
      float preDB = preSpectrum(freq);
      if (preDB <= constants::MINUS_INFINITY_DB) {
        return preDB;
      }
      return preDB + freqResponse(freq);
    };
  }

  const auto spectrumToXY = [&](std::function<float(float)> &spectrum) {
    return [&](float x) {
      const float freq = xToFreq(x);
      return freq > halfSampleRate ? yMax : dBToY(spectrum(freq));
    };
  };

  {
    const auto preSpectrumXY = spectrumToXY(preSpectrum);

    juce::Path preSpectrumPath;
    preSpectrumPath.preallocateSpace(3 * (static_cast<int>(xMax) + 3));

    preSpectrumPath.startNewSubPath(xMin, preSpectrumXY(xMin));
    for (float x = xMin + 1.0f; x < xMax; ++x) {
      preSpectrumPath.lineTo(x, preSpectrumXY(x));
    }
    preSpectrumPath.lineTo(xMax, yMax);
    preSpectrumPath.lineTo(xMin, yMax);
    preSpectrumPath.closeSubPath();

    juce::ColourGradient gradient(
        juce::Colours::transparentWhite, juce::Point(0.0f, yMax),
        juce::Colour(0xff87bfff), juce::Point(0.0f, yMin), false);

    g.setGradientFill(gradient);
    g.fillPath(preSpectrumPath);
  }

  {
    const auto postSpectrumXY = spectrumToXY(postSpectrum);

    juce::Path postSpectrumPath;
    postSpectrumPath.preallocateSpace(3 * static_cast<int>(xMax));

    postSpectrumPath.startNewSubPath(xMin, postSpectrumXY(xMin));
    for (float x = xMin + 1.0f; x < xMax; ++x) {
      postSpectrumPath.lineTo(x, postSpectrumXY(x));
    }

    g.setColour(juce::Colour(0xff87bfff));
    g.strokePath(
        postSpectrumPath,
        juce::PathStrokeType(2.f, juce::PathStrokeType::JointStyle::curved,
                             juce::PathStrokeType::EndCapStyle::rounded));
  }
}

void AnalyzerComponent::timerCallback() {
  auto analyzerProcessor = pluginProcessor.getAnalyzerProcessor();
  if (analyzerProcessor) {
    analyzerProcessor->updateAmplitudes();
  }

  repaint();
}