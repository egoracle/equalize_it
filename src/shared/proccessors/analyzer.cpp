#define _USE_MATH_DEFINES

#include "analyzer.hpp"
#include "../math/math.hpp"

#include <spline.h>

AnalyzerProcessor::AnalyzerProcessor()
    : audioBlock(FFT_SIZE), window(dsp::nuttallWindow(FFT_SIZE)),
      amplitudes(FFT_SIZE / 2) {
  for (auto &amplitude : amplitudes) {
    amplitude.setCurrentAndTargetValue(juce::Decibels::gainToDecibels(0.0f));
  }
}

void AnalyzerProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);

  for (auto &amplitude : amplitudes) {
    amplitude.reset(sampleRate, 0.3);
  }
}

void AnalyzerProcessor::processBlock(juce::AudioSampleBuffer &audioBuffer,
                                     juce::MidiBuffer &midiBuffer) {
  BaseProcessor::processBlock(audioBuffer, midiBuffer);

  for (auto &amplitude : amplitudes) {
    amplitude.skip(audioBuffer.getNumSamples());
  }

  if (audioBuffer.getNumChannels() > 0) {
    const float *channelDataLeft = audioBuffer.getReadPointer(0);
    const float *channelDataRight = audioBuffer.getReadPointer(1);

    for (auto i = 0; i < audioBuffer.getNumSamples(); ++i) {
      auto monoSample = (channelDataLeft[i] + channelDataRight[i]) / 2;

      pushNextSampleToAudioBlock(monoSample);
    }
  }
}

void AnalyzerProcessor::pushNextSampleToAudioBlock(float sample) {
  if (audioBlockIndex == FFT_SIZE) {
    nextBlockReady = true;

    for (auto i = 0; i < FFT_SIZE - HOP_SIZE; ++i) {
      audioBlock[i] = audioBlock[HOP_SIZE + i - 1];
    }

    audioBlockIndex = FFT_SIZE - HOP_SIZE;
  }

  audioBlock[audioBlockIndex] = std::complex(sample, 0.0f);
  audioBlockIndex++;
}

void AnalyzerProcessor::updateAmplitudes() {
  if (!nextBlockReady) {
    return;
  }

  auto phasors = window * audioBlock;
  dsp::fft(phasors);

  phasors = math::octaveSmoothing(12, phasors);

  for (int i = 0; i < FFT_SIZE / 2; ++i) {
    float amplitude =
        juce::Decibels::gainToDecibels(std::abs(phasors[i]) / FFT_SIZE);

    if (amplitude < amplitudes[i].getCurrentValue()) {
      amplitudes[i].setTargetValue(amplitude);
    } else {
      amplitudes[i].setCurrentAndTargetValue(amplitude);
    }
  }

  nextBlockReady = false;
}

std::valarray<juce::LinearSmoothedValue<float>> &
AnalyzerProcessor::getAmplitudes() {
  return amplitudes;
}