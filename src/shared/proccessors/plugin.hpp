#pragma once

#include "gain.hpp"
#include "lib/lib.hpp"

class PluginProcessor : public CascadeProcessor {
public:
  PluginProcessor(
      std::function<juce::AudioProcessorEditor *(PluginProcessor *)> &);

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;
  int getNumPrograms() override;

  void getStateInformation(juce::MemoryBlock &) override;
  void setStateInformation(const void *, int) override;

  GainProcessor *getGainProcessor();
  juce::AudioProcessorValueTreeState &getAPVTS();

protected:
  void initializeEffectNodes() override;
  void connectAudioNodes() override;

private:
  juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
  std::function<juce::AudioProcessorEditor *(PluginProcessor *)>
      createEditorCallback;

  juce::AudioProcessorValueTreeState apvts;

  Node::Ptr gainNode;
};
