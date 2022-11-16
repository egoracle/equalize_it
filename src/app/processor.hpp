#pragma once

#include "../shared/shared.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

class AppProcessor : public BaseProcessor {
public:
  using AudioGraphIOProcessor =
      juce::AudioProcessorGraph::AudioGraphIOProcessor;
  using Node = juce::AudioProcessorGraph::Node;

public:
  AppProcessor();

  const juce::String getName() const override;
  int getNumPrograms() override;
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  bool hasEditor() const override;
  juce::AudioProcessorEditor *createEditor() override;

  void initializeGraph();
  void connectAudioNodes();

private:
  std::unique_ptr<juce::AudioProcessorGraph> processorGraph;

  Node::Ptr audioInputNode;
  Node::Ptr audioOutputNode;
};