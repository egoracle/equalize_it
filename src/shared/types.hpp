#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace types {

using APVTS = juce::AudioProcessorValueTreeState;

enum class FilterType { LowPass, Peak, HighPass };

} // namespace types
