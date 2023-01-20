#pragma once

#include "base_processor.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace shared::lib {

class BaseComponent : public juce::Component {
public:
  using Track = juce::Grid::TrackInfo;
  using Fr = juce::Grid::Fr;
  using Px = juce::Grid::Px;

public:
  BaseComponent() {}
  virtual ~BaseComponent() {}

  virtual void paint(juce::Graphics &g) {}
  virtual void resized() { grid.performLayout(getLocalBounds()); }

protected:
  juce::Grid grid;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseComponent)
};

} // namespace shared::lib