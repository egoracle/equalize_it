#include "app.hpp"

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  std::function<juce::AudioProcessorEditor *(PluginProcessor *)>
      createEditorCallback = [](PluginProcessor *processor) {
        return new PluginEditor(*processor);
      };

  return new PluginProcessor(createEditorCallback);
}