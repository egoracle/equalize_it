#include "processor.hpp"
#include "editor.hpp"

AppProcessor::AppProcessor() : BaseProcessor() {}

const juce::String AppProcessor::getName() const { return JucePlugin_Name; }

int AppProcessor::getNumPrograms() { return 1; }

bool AppProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *AppProcessor::createEditor() {
  return new AppEditor(*this);
}
