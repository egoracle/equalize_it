#include "app/processor.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new AppProcessor();
}