#pragma once

#include "lib/cascade_processor.hpp"

struct EqualizerParameters {
  juce::AudioParameterBool *isActive;

  static void addToLayout(APVTS::ParameterLayout &);

  static juce::String getIsActiveID() noexcept;
  static juce::String getIsActiveName() noexcept;
  bool getIsActiveValue();
};

class EqualizerProcessor : public CascadeProcessor {
public:
  EqualizerProcessor(APVTS &);

  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

protected:
  void initializeEffectNodes() override;
  void connectAudioNodes() override;

private:
  EqualizerParameters extractEqualizerParameters(APVTS &);

  APVTS &treeState;
  EqualizerParameters params;
  Node::Ptr filterNodes[12];
};