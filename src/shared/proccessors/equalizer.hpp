#pragma once

#include "filter.hpp"
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
  enum { FILTERS_COUNT = 12 };

  EqualizerProcessor(APVTS &);

  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  FilterProcessor *getFilter(int);

  std::function<float(float)> getFrequencyResponse() override;

protected:
  void initializeEffectNodes() override;
  void connectAudioNodes() override;

private:
  EqualizerParameters extractEqualizerParameters(APVTS &);

  APVTS &treeState;
  EqualizerParameters params;
  Node::Ptr filterNodes[FILTERS_COUNT];
};