#include "plugin.hpp"

PluginProcessor::PluginProcessor(
    std::function<juce::AudioProcessorEditor *(PluginProcessor *)> &callback)
    : createEditorCallback(callback),
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {}

juce::AudioProcessorEditor *PluginProcessor::createEditor() {
  return createEditorCallback(this);
}

bool PluginProcessor::hasEditor() const { return true; }

const juce::String PluginProcessor::getName() const { return JucePlugin_Name; }

int PluginProcessor::getNumPrograms() { return 1; }

void PluginProcessor::getStateInformation(juce::MemoryBlock &) {}
void PluginProcessor::setStateInformation(const void *, int) {}

GainProcessor *PluginProcessor::getGainProcessor() {
  if (gainNode) {
    return dynamic_cast<GainProcessor *>(gainNode.get()->getProcessor());
  }
  return nullptr;
}

EqualizerProcessor *PluginProcessor::getEqualizerProcessor() {
  if (equalizerNode) {
    return dynamic_cast<EqualizerProcessor *>(
        equalizerNode.get()->getProcessor());
  }

  return nullptr;
}

APVTS &PluginProcessor::getAPVTS() { return apvts; }

void PluginProcessor::initializeEffectNodes() {
  gainNode = audioGraph->addNode(std::make_unique<GainProcessor>(apvts));
  equalizerNode =
      audioGraph->addNode(std::make_unique<EqualizerProcessor>(apvts));
}

void PluginProcessor::connectAudioNodes() {
  for (int channel = 0; channel < 2; ++channel) {
    audioGraph->addConnection(
        {{audioInputNode->nodeID, channel}, {equalizerNode->nodeID, channel}});
    audioGraph->addConnection(
        {{equalizerNode->nodeID, channel}, {gainNode->nodeID, channel}});
    audioGraph->addConnection(
        {{gainNode->nodeID, channel}, {audioOutputNode->nodeID, channel}});
  }
}

APVTS::ParameterLayout PluginProcessor::createParameterLayout() {
  APVTS::ParameterLayout layout;

  GainParameters::addToLayout(layout);
  EqualizerParameters::addToLayout(layout);

  return layout;
}
