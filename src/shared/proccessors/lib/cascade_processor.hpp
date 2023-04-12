#pragma once

#include "base_processor.hpp"

class CascadeProcessor : public BaseProcessor {
public:
  using AudioGraphIOProcessor =
      juce::AudioProcessorGraph::AudioGraphIOProcessor;
  using Node = juce::AudioProcessorGraph::Node;

  CascadeProcessor() : audioGraph(new juce::AudioProcessorGraph()) {}

  virtual void prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);

    audioGraph->setPlayConfigDetails(getMainBusNumInputChannels(),
                                     getMainBusNumOutputChannels(), sampleRate,
                                     samplesPerBlock);
    audioGraph->prepareToPlay(sampleRate, samplesPerBlock);

    initializeGraph();
  }

  virtual void releaseResources() { audioGraph->releaseResources(); }

  virtual void processBlock(juce::AudioSampleBuffer &audioBuffer,
                            juce::MidiBuffer &midiBuffer) {
    BaseProcessor::processBlock(audioBuffer, midiBuffer);

    updateGraph();

    audioGraph->processBlock(audioBuffer, midiBuffer);
  }

private:
  void initializeGraph() {
    audioGraph->clear();

    audioInputNode =
        audioGraph->addNode(std::make_unique<AudioGraphIOProcessor>(
            AudioGraphIOProcessor::audioInputNode));
    audioOutputNode =
        audioGraph->addNode(std::make_unique<AudioGraphIOProcessor>(
            AudioGraphIOProcessor::audioOutputNode));

    initializeEffectNodes();
    connectAudioNodes();
  }

protected:
  virtual void initializeEffectNodes() {}

  virtual void updateGraph() {}

  virtual void connectAudioNodes() {
    for (int channel = 0; channel < 2; ++channel) {
      audioGraph->addConnection({{audioInputNode->nodeID, channel},
                                 {audioOutputNode->nodeID, channel}});
    }
  }

protected:
  std::unique_ptr<juce::AudioProcessorGraph> audioGraph;

  Node::Ptr audioInputNode;
  Node::Ptr audioOutputNode;
};