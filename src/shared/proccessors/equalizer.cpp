#include "equalizer.hpp"

EqualizerProcessor::EqualizerProcessor(types::APVTS &apvts)
    : treeState(apvts), params(apvts) {}

void EqualizerProcessor::processBlock(juce::AudioSampleBuffer &audioBuffer,
                                      juce::MidiBuffer &midiBuffer) {
  if (params.getIsActiveValue()) {
    CascadeProcessor::processBlock(audioBuffer, midiBuffer);
  } else {
    BaseProcessor::processBlock(audioBuffer, midiBuffer);
  }
}

FilterProcessor *EqualizerProcessor::getFilter(int filterID) {
  jassert(filterID >= constants::FILTER_MIN_ID &&
          filterID <= constants::FILTER_MAX_ID);

  return dynamic_cast<FilterProcessor *>(
      filterNodes[filterID - 1]->getProcessor());
}

void EqualizerProcessor::initializeEffectNodes() {
  for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
       ++id) {
    filterNodes[id - 1] =
        audioGraph->addNode(std::make_unique<FilterProcessor>(id, treeState));
  }
}

void EqualizerProcessor::connectAudioNodes() {
  for (int channel = 0; channel < 2; ++channel) {
    audioGraph->addConnection(
        {{audioInputNode->nodeID, channel}, {filterNodes[0]->nodeID, channel}});
    for (int i = 0; i < constants::FILTERS_COUNT - 1; ++i) {
      audioGraph->addConnection({{filterNodes[i]->nodeID, channel},
                                 {filterNodes[i + 1]->nodeID, channel}});
    }
    audioGraph->addConnection(
        {{filterNodes[constants::FILTERS_COUNT - 1]->nodeID, channel},
         {audioOutputNode->nodeID, channel}});
  }
}

std::function<float(float)> EqualizerProcessor::getFrequencyResponse() {
  std::vector<std::function<float(float)>> filterResponses;

  for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
       ++id) {
    filterResponses.push_back(getFilter(id)->getFrequencyResponse());
  }

  return [&, filterResponses](float freq) {
    float response = 0;
    for (auto &filterResponse : filterResponses) {
      response += filterResponse(freq);
    }
    return response;
  };
}
