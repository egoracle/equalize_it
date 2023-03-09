#pragma once

#include "shared.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

class AppProcessor : public BaseProcessor {
public:
  AppProcessor();

  const juce::String getName() const override;
  int getNumPrograms() override;
  bool hasEditor() const override;
  juce::AudioProcessorEditor *createEditor() override;
};
