#include "plugin.hpp"

PluginProcessor::PluginProcessor(
    std::function<juce::AudioProcessorEditor *(PluginProcessor *)> &callback)
    : createEditorCallback(callback),
      apvts(*this, nullptr, "Parameters", createParameterLayout()),
      CascadeProcessor() {}

juce::AudioProcessorEditor *PluginProcessor::createEditor() {
  return createEditorCallback(this);
}

bool PluginProcessor::hasEditor() const { return true; }

const juce::String PluginProcessor::getName() const { return JucePlugin_Name; }

int PluginProcessor::getNumPrograms() { return 1; }

void PluginProcessor::getStateInformation(juce::MemoryBlock &) {}
void PluginProcessor::setStateInformation(const void *, int) {}

GainProcessor *PluginProcessor::getGainProcessor() {
  return (GainProcessor *)gainNode.get()->getProcessor();
}

juce::AudioProcessorValueTreeState &PluginProcessor::getAPVTS() {
  return apvts;
}

void PluginProcessor::initializeEffectNodes() {
  gainNode = audioGraph->addNode(std::make_unique<GainProcessor>(apvts));
}

void PluginProcessor::connectAudioNodes() {
  for (int channel = 0; channel < 2; ++channel) {
    audioGraph->addConnection(
        {{audioInputNode->nodeID, channel}, {gainNode->nodeID, channel}});
    audioGraph->addConnection(
        {{gainNode->nodeID, channel}, {audioOutputNode->nodeID, channel}});
  }
}

juce::AudioProcessorValueTreeState::ParameterLayout
PluginProcessor::createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;

  layout.add(std::make_unique<juce::AudioParameterFloat>("gain", "Gain", -24.0f,
                                                         24.0f, 0.0f));

  return layout;
}
