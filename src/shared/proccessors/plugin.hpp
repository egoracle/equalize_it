#pragma once

#include "filter.hpp"
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
  APVTS &getAPVTS();

protected:
  void initializeEffectNodes() override;
  void connectAudioNodes() override;

private:
  APVTS::ParameterLayout createParameterLayout();

private:
  std::function<juce::AudioProcessorEditor *(PluginProcessor *)>
      createEditorCallback;

  APVTS apvts;

  Node::Ptr gainNode;
  Node::Ptr filterNode;
};
