#pragma once

#include "analyzer/processor.hpp"
#include "shared.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

namespace app {

class AppProcessor : public shared::lib::BaseProcessor {
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

  AnalyzerProcessor &getAnalyzerProcessor();

private:
  std::unique_ptr<juce::AudioProcessorGraph> processorGraph;

  Node::Ptr audioInputNode;
  Node::Ptr analyzerNode;
  Node::Ptr audioOutputNode;
};

} // namespace app