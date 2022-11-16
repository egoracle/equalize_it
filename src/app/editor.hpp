#pragma once

#include "../shared/shared.hpp"
#include "../widgets/widgets.hpp"
#include "processor.hpp"

class AppEditor : public BaseEditor {
public:
  AppEditor(AppProcessor &audioProcessor);

  void paint(juce::Graphics &g) override;

private:
  Header header;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppEditor)
};