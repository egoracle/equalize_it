#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <map>

#include "../types.hpp"
#include "analyzer.hpp"
#include "cascade_processor.hpp"
#include "equalizer.hpp"
#include "gain.hpp"

class PluginProcessor : public CascadeProcessor {
public:
  struct UiState {
    int selectedFilterID;
    std::map<int, bool> usedFilterIDs;

    UiState();
    UiState(int, juce::Array<int> &);

    bool hasSelectedFilter();
    bool hasUsedFilters();

    void selectPrevFilter();
    void selectNextFilter();

    bool addFilter();
    void removeFilter(int);

    juce::var toVar() const;
    static UiState fromVar(const juce::var &);
  };

  PluginProcessor(
      std::function<juce::AudioProcessorEditor *(PluginProcessor *)> &);

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;
  int getNumPrograms() override;

  void getStateInformation(juce::MemoryBlock &) override;
  void setStateInformation(const void *, int) override;

  AnalyzerProcessor *getAnalyzerProcessor();
  GainProcessor *getGainProcessor();
  EqualizerProcessor *getEqualizerProcessor();

  types::APVTS &getAPVTS();
  UiState &getUiState();

protected:
  void initializeEffectNodes() override;
  void connectAudioNodes() override;

private:
  types::APVTS::ParameterLayout createParameterLayout();

  std::function<juce::AudioProcessorEditor *(PluginProcessor *)>
      createEditorCallback;

  types::APVTS apvts;
  UiState uiState;

  Node::Ptr analyzerNode;
  Node::Ptr gainNode;
  Node::Ptr equalizerNode;
};
