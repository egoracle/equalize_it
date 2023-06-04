#include "plugin.hpp"

#include "../constants.hpp"

// UiState

PluginProcessor::UiState::UiState() : selectedFilterID(-1) {
  for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
       ++id) {
    usedFilterIDs.insert(std::pair<int, bool>(id, false));
  }
}

bool PluginProcessor::UiState::hasSelectedFilter() {
  return selectedFilterID != -1;
}

bool PluginProcessor::UiState::hasUsedFilters() {
  for (const auto &[filterID, isUsed] : usedFilterIDs) {
    if (isUsed) {
      return true;
    }
  }

  return false;
}

PluginProcessor::UiState::UiState(int sfi, juce::Array<int> &filterIDs)
    : selectedFilterID(sfi) {
  for (int id = constants::FILTER_MIN_ID; id <= constants::FILTER_MAX_ID;
       ++id) {
    usedFilterIDs.insert(std::pair<int, bool>(id, false));
  }
  for (const auto &filterID : filterIDs) {
    usedFilterIDs[filterID] = true;
  }
}

void PluginProcessor::UiState::selectPrevFilter() {
  jassert(hasSelectedFilter());

  auto it = usedFilterIDs.find(selectedFilterID);
  if (it == usedFilterIDs.begin()) {
    return;
  }

  do {
    --it;
    if (it->second) {
      selectedFilterID = it->first;
      break;
    }
  } while (it != usedFilterIDs.begin());
}

void PluginProcessor::UiState::selectNextFilter() {
  jassert(hasSelectedFilter());

  for (auto it = std::next(usedFilterIDs.find(selectedFilterID));
       it != usedFilterIDs.end(); ++it) {
    if (it->second) {
      selectedFilterID = it->first;
      break;
    }
  }
}

bool PluginProcessor::UiState::addFilter() {
  for (const auto &[id, isUsed] : usedFilterIDs) {
    if (!isUsed) {
      selectedFilterID = id;
      usedFilterIDs[id] = true;
      return true;
    }
  }

  return false;
}

void PluginProcessor::UiState::removeFilter(int filterID) {
  usedFilterIDs[filterID] = false;

  selectedFilterID = -1;
  for (const auto &[id, isUsed] : usedFilterIDs) {
    if (isUsed) {
      selectedFilterID = id;
    }
  }
}

juce::var PluginProcessor::UiState::toVar() const {
  juce::var filterIDs;
  for (const auto &[id, isUsed] : usedFilterIDs) {
    if (isUsed) {
      filterIDs.append(id);
    }
  }

  juce::var var;

  var.getDynamicObject()->setProperty("selectedFilterID", selectedFilterID);
  var.getDynamicObject()->setProperty("filterIDs", filterIDs);

  return var;
}

PluginProcessor::UiState
PluginProcessor::UiState::fromVar(const juce::var &var) {
  if (var.isObject()) {
    int selectedFilterID = var.getProperty("selectedFilterID", -1);

    juce::Array<int> filterIDs;
    auto varFilterIDs = var.getProperty("filterIDs", juce::var());
    auto refFilterIDs = varFilterIDs.getArray();
    for (const auto &refFilterID : *refFilterIDs) {
      filterIDs.add(refFilterID);
    }

    return UiState(selectedFilterID, filterIDs);
  }

  return UiState();
}

// PluginProcessor

PluginProcessor::PluginProcessor(
    std::function<juce::AudioProcessorEditor *(PluginProcessor *)> &callback)
    : createEditorCallback(callback),
      apvts(*this, nullptr, "ParametersState", createParameterLayout()) {}

juce::AudioProcessorEditor *PluginProcessor::createEditor() {
  return createEditorCallback(this);
}

bool PluginProcessor::hasEditor() const { return true; }

const juce::String PluginProcessor::getName() const { return JucePlugin_Name; }

int PluginProcessor::getNumPrograms() { return 1; }

void PluginProcessor::getStateInformation(juce::MemoryBlock &destData) {
  return;
  juce::ValueTree uiStateTree("UiState");
  uiStateTree.setProperty("UiStateObject", uiState.toVar(), nullptr);

  juce::ValueTree pluginStateTree("PluginState");
  pluginStateTree.appendChild(apvts.copyState(), nullptr);
  pluginStateTree.appendChild(uiStateTree, nullptr);

  std::unique_ptr<juce::XmlElement> xml(pluginStateTree.createXml());
  copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
  return;
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));

  if (!xmlState.get() || !xmlState->hasTagName("PluginState")) {
    return;
  }

  juce::ValueTree pluginStateTree(juce::ValueTree::fromXml(*xmlState));
  apvts.replaceState(pluginStateTree.getChildWithName(apvts.state.getType()));
  uiState = UiState::fromVar(
      pluginStateTree.getChildWithName("UiState").getProperty("UiStateObject"));
}

AnalyzerProcessor *PluginProcessor::getAnalyzerProcessor() {
  if (analyzerNode) {
    return dynamic_cast<AnalyzerProcessor *>(
        analyzerNode.get()->getProcessor());
  }

  return nullptr;
}

GainProcessor *PluginProcessor::getGainProcessor() {
  if (gainNode) {
    return dynamic_cast<GainProcessor *>(gainNode.get()->getProcessor());
  }
  return nullptr;
}

EqualizerProcessor *PluginProcessor::getEqualizerProcessor() {
  if (equalizerNode) {
    return dynamic_cast<EqualizerProcessor *>(
        equalizerNode.get()->getProcessor());
  }

  return nullptr;
}

types::APVTS &PluginProcessor::getAPVTS() { return apvts; }
PluginProcessor::UiState &PluginProcessor::getUiState() { return uiState; }

void PluginProcessor::initializeEffectNodes() {
  analyzerNode = audioGraph->addNode(std::make_unique<AnalyzerProcessor>());
  gainNode = audioGraph->addNode(std::make_unique<GainProcessor>(apvts));
  equalizerNode =
      audioGraph->addNode(std::make_unique<EqualizerProcessor>(apvts));
}

void PluginProcessor::connectAudioNodes() {
  for (int channel = 0; channel < 2; ++channel) {
    audioGraph->addConnection(
        {{audioInputNode->nodeID, channel}, {analyzerNode->nodeID, channel}});
    audioGraph->addConnection(
        {{analyzerNode->nodeID, channel}, {equalizerNode->nodeID, channel}});
    audioGraph->addConnection(
        {{equalizerNode->nodeID, channel}, {gainNode->nodeID, channel}});
    audioGraph->addConnection(
        {{gainNode->nodeID, channel}, {audioOutputNode->nodeID, channel}});
  }
}

types::APVTS::ParameterLayout PluginProcessor::createParameterLayout() {
  types::APVTS::ParameterLayout layout;

  GainParameters::addToLayout(layout);
  EqualizerParameters::addToLayout(layout);

  return layout;
}
