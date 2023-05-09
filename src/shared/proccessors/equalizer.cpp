#include "equalizer.hpp"

// EqualizerParameters

void EqualizerParameters::addToLayout(APVTS::ParameterLayout &layout) {
  layout.add(std::make_unique<juce::AudioParameterBool>(
      getIsActiveID(), getIsActiveName(), true));

  for (int i = 1; i <= EqualizerProcessor::FILTERS_COUNT; ++i) {
    FilterParameters::addToLayout(layout, i);
  }
}

juce::String EqualizerParameters::getIsActiveID() noexcept {
  return "equalizer_active";
}

juce::String EqualizerParameters::getIsActiveName() noexcept {
  return "Equalizer Active";
}

bool EqualizerParameters::getIsActiveValue() {
  jassert(isActive != nullptr);

  return isActive->get();
}

// EqualizerProcessor

EqualizerProcessor::EqualizerProcessor(APVTS &apvts)
    : treeState(apvts), params(extractEqualizerParameters(apvts)) {}

void EqualizerProcessor::processBlock(juce::AudioSampleBuffer &audioBuffer,
                                      juce::MidiBuffer &midiBuffer) {
  if (params.getIsActiveValue()) {
    CascadeProcessor::processBlock(audioBuffer, midiBuffer);
  } else {
    BaseProcessor::processBlock(audioBuffer, midiBuffer);
  }
}

FilterProcessor *EqualizerProcessor::getFilter(int filterID) {
  jassert(filterID >= 1 && filterID <= FILTERS_COUNT);

  int index = filterID - 1;

  return dynamic_cast<FilterProcessor *>(filterNodes[index]->getProcessor());
}

void EqualizerProcessor::initializeEffectNodes() {
  for (int i = 0; i < FILTERS_COUNT; ++i) {
    filterNodes[i] = audioGraph->addNode(
        std::make_unique<FilterProcessor>(i + 1, treeState));
  }
}

void EqualizerProcessor::connectAudioNodes() {
  for (int channel = 0; channel < 2; ++channel) {
    audioGraph->addConnection(
        {{audioInputNode->nodeID, channel}, {filterNodes[0]->nodeID, channel}});
    for (int i = 0; i < FILTERS_COUNT - 1; ++i) {
      audioGraph->addConnection({{filterNodes[i]->nodeID, channel},
                                 {filterNodes[i + 1]->nodeID, channel}});
    }
    audioGraph->addConnection(
        {{filterNodes[FILTERS_COUNT - 1]->nodeID, channel},
         {audioOutputNode->nodeID, channel}});
  }
}

std::function<float(float)> EqualizerProcessor::getFrequencyResponse() {
  return [&](float freq) {
    float response = this->getFilter(1)->getFrequencyResponse()(freq);

    for (int i = 2; i <= FILTERS_COUNT; ++i) {
      response += this->getFilter(i)->getFrequencyResponse()(freq);
    }

    return response;
  };
}

EqualizerParameters
EqualizerProcessor::extractEqualizerParameters(APVTS &apvts) {
  EqualizerParameters p;

  p.isActive = dynamic_cast<juce::AudioParameterBool *>(
      apvts.getParameter(EqualizerParameters::getIsActiveID()));

  return p;
}