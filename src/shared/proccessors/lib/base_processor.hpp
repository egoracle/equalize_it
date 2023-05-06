#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <functional>

using APVTS = juce::AudioProcessorValueTreeState;

class BaseProcessor : public juce::AudioProcessor {
public:
  BaseProcessor()
      : AudioProcessor(
            BusesProperties()
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

  virtual bool isBusesLayoutSupported(const BusesLayout &layouts) const {
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled() ||
        layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
      return false;

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
      return false;

    return layouts.getMainInputChannelSet() ==
           layouts.getMainOutputChannelSet();
  }

  virtual void prepareToPlay(double, int) {}
  virtual void releaseResources() {}
  virtual void processBlock(juce::AudioSampleBuffer &audioBuffer,
                            juce::MidiBuffer &) {
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels();
         ++i) {
      audioBuffer.clear(i, 0, audioBuffer.getNumSamples());
    }
  }

  virtual juce::AudioProcessorEditor *createEditor() { return nullptr; }
  virtual bool hasEditor() const { return false; }

  virtual const juce::String getName() const { return {}; }
  bool acceptsMidi() const { return false; }
  bool producesMidi() const { return false; }
  virtual double getTailLengthSeconds() const { return 0.0; }

  virtual int getNumPrograms() { return 0; }
  virtual int getCurrentProgram() { return 0; }
  virtual void setCurrentProgram(int) {}
  virtual const juce::String getProgramName(int) { return {}; }
  virtual void changeProgramName(int, const juce::String &) {}

  virtual void getStateInformation(juce::MemoryBlock &) {}
  virtual void setStateInformation(const void *, int) {}

  virtual std::function<float(float)> getFrequencyResponse() {
    return [](float) { return juce::Decibels::gainToDecibels(1.0f); };
  }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseProcessor)
};