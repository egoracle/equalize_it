#pragma once

#include "../constants.hpp"
#include "../parameters/equalizer.hpp"
#include "../types.hpp"
#include "cascade_processor.hpp"
#include "filter.hpp"

class EqualizerProcessor : public CascadeProcessor {
public:
  EqualizerProcessor(types::APVTS &);

  void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

  FilterProcessor *getFilter(int);

  std::function<float(float)> getFrequencyResponse() override;

protected:
  void initializeEffectNodes() override;
  void connectAudioNodes() override;

private:
  types::APVTS &treeState;
  EqualizerParameters params;
  Node::Ptr filterNodes[constants::FILTERS_COUNT];
};