#include "processor.hpp"
#include "editor.hpp"

AppProcessor::AppProcessor()
    : BaseProcessor(), processorGraph(new juce::AudioProcessorGraph()) {}

const juce::String AppProcessor::getName() const { return JucePlugin_Name; }

int AppProcessor::getNumPrograms() { return 1; }

void AppProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  processorGraph->setPlayConfigDetails(getMainBusNumInputChannels(),
                                       getMainBusNumOutputChannels(),
                                       sampleRate, samplesPerBlock);

  processorGraph->prepareToPlay(sampleRate, samplesPerBlock);

  initializeGraph();
}

void AppProcessor::releaseResources() { processorGraph->releaseResources(); }

bool AppProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *AppProcessor::createEditor() {
  return new AppEditor(*this);
}

void AppProcessor::initializeGraph() {
  processorGraph->clear();

  audioInputNode =
      processorGraph->addNode(std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::audioInputNode));
  equalizerNode =
      processorGraph->addNode(std::make_unique<EqualizerProcessor>());
  audioOutputNode =
      processorGraph->addNode(std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::audioOutputNode));

  connectAudioNodes();
}

void AppProcessor::connectAudioNodes() {
  for (int channel = 0; channel < 2; ++channel) {
    processorGraph->addConnection(
        {{audioInputNode->nodeID, channel}, {equalizerNode->nodeID, channel}});
    processorGraph->addConnection(
        {{equalizerNode->nodeID, channel}, {audioOutputNode->nodeID, channel}});
  }
}

EqualizerProcessor &AppProcessor::getEqualizerProcessor() {
  return *dynamic_cast<EqualizerProcessor *>(
      equalizerNode.get()->getProcessor());
}
