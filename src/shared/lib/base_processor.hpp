#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class BaseProcessor : public juce::AudioProcessor {
public:
  BaseProcessor()
      : AudioProcessor(
            BusesProperties()
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}
  ~BaseProcessor() {}

  virtual void prepareToPlay(double sampleRate, int samplesPerBlock) {}
  virtual void releaseResources() {}
  virtual void processBlock(juce::AudioSampleBuffer &buffer,
                            juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
      buffer.clear(i, 0, buffer.getNumSamples());
    }
  }

  virtual juce::AudioProcessorEditor *createEditor() { return nullptr; }
  virtual bool hasEditor() const { return false; }

  virtual const juce::String getName() const { return {}; }
  bool acceptsMidi() const { return false; }
  bool producesMidi() const { return false; }
  virtual double getTailLengthSeconds() const { return 0.0; };

  virtual int getNumPrograms() { return 0; }
  virtual int getCurrentProgram() { return 0; }
  virtual void setCurrentProgram(int index) {}
  virtual const juce::String getProgramName(int index) { return {}; }
  virtual void changeProgramName(int index, const juce::String &newName) {}

  virtual void getStateInformation(juce::MemoryBlock &destData) {}
  virtual void setStateInformation(const void *data, int sizeInBytes) {}

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseProcessor)
};