
#include "analyzer.hpp"
#include "../constants.hpp"
#include "../math/math.hpp"

AnalyzerProcessor::AnalyzerProcessor() { update(); }

void AnalyzerProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);

  for (auto &amplitude : amplitudes) {
    amplitude.reset(sampleRate, rampLengthAmplitudes);
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

void AnalyzerProcessor::changeFFTOrder(int newOrder) {
  jassert(newOrder >= 10 && newOrder <= 13);

  fftOrder = newOrder;
  fftSize = 1 << newOrder;

  update();
}

void AnalyzerProcessor::changeWindowKind(dsp::WindowKind kind) {
  windowKind = kind;

  update();
}

void AnalyzerProcessor::update() {
  audioBlock.resize(fftSize);
  audioBlockIndex = 0;
  nextBlockReady = false;

  phasors.resize(fftSize * 2);
  if (window == nullptr || window->getKind() != windowKind) {
    window =
        std::unique_ptr<dsp::Window>(dsp::Window::createWindow(windowKind));
  }
  window->updateCoefficients(fftSize);

  amplitudes.resize(fftSize);
  for (auto &amplitude : amplitudes) {
    amplitude.setCurrentAndTargetValue(constants::MINUS_INFINITY_DB);
  }

  double sampleRate = getSampleRate();

  if (sampleRate != 0) {
    for (auto &amplitude : amplitudes) {
      amplitude.reset(sampleRate, rampLengthAmplitudes);
    }
  }
}

void AnalyzerProcessor::pushNextSampleToAudioBlock(float sample) {
  if (audioBlockIndex == fftSize) {
    if (!nextBlockReady) {
      moveAudioBlockSamples();
      nextBlockReady = true;
    }

    audioBlockIndex = 0;
  }

  audioBlock[audioBlockIndex] = sample;
  audioBlockIndex++;
}

void AnalyzerProcessor::moveAudioBlockSamples() {
  for (int i = 0; i < audioBlock.size(); ++i) {
    phasors[i] = std::complex<float>(audioBlock[i], 0.0f);
  }
  for (int i = audioBlock.size(); i < phasors.size(); ++i) {
    phasors[i] = std::complex<float>(0.0f, 0.0f);
  }
}

void AnalyzerProcessor::updateAmplitudes() {
  if (!nextBlockReady) {
    return;
  }

  phasors = window->getCoefficients() * phasors;
  dsp::fft(phasors);

  phasors = math::octaveSmoothing(24, phasors);

  for (int i = 0; i < amplitudes.size(); ++i) {
    float amplitude =
        juce::Decibels::gainToDecibels(std::abs(phasors[i]) / fftSize);

    if (amplitude < amplitudes[i].getCurrentValue()) {
      amplitudes[i].setTargetValue(amplitude);
    } else {
      amplitudes[i].setCurrentAndTargetValue(amplitude);
    }
  }

  nextBlockReady = false;
}
