#include "processor.hpp"
#include "editor.hpp"

app::AppProcessor::AppProcessor()
    : BaseProcessor(), processorGraph(new juce::AudioProcessorGraph()) {}

const juce::String app::AppProcessor::getName() const {
  return JucePlugin_Name;
}

int app::AppProcessor::getNumPrograms() { return 1; }

void app::AppProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  processorGraph->setPlayConfigDetails(getMainBusNumInputChannels(),
                                       getMainBusNumOutputChannels(),
                                       sampleRate, samplesPerBlock);

  processorGraph->prepareToPlay(sampleRate, samplesPerBlock);

  initializeGraph();
}

void app::AppProcessor::releaseResources() {
  processorGraph->releaseResources();
}

bool app::AppProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *app::AppProcessor::createEditor() {
  return new AppEditor(*this);
}

void app::AppProcessor::initializeGraph() {
  processorGraph->clear();

  audioInputNode =
      processorGraph->addNode(std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::audioInputNode));
  analyzerNode = processorGraph->addNode(std::make_unique<AnalyzerProcessor>());
  audioOutputNode =
      processorGraph->addNode(std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::audioOutputNode));

  connectAudioNodes();
}

void app::AppProcessor::connectAudioNodes() {
  for (int channel = 0; channel < 2; ++channel) {
    processorGraph->addConnection(
        {{audioInputNode->nodeID, channel}, {analyzerNode->nodeID, channel}});
    processorGraph->addConnection(
        {{analyzerNode->nodeID, channel}, {audioOutputNode->nodeID, channel}});
  }
}

AnalyzerProcessor &app::AppProcessor::getAnalyzerProcessor() {
  return *dynamic_cast<AnalyzerProcessor *>(analyzerNode.get()->getProcessor());
}
