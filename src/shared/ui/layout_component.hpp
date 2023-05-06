#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class LayoutComponent : public juce::Component {
public:
  using Track = juce::Grid::TrackInfo;
  using Fr = juce::Grid::Fr;
  using Px = juce::Grid::Px;

public:
  LayoutComponent() {}
  virtual ~LayoutComponent() {}

  virtual void paint(juce::Graphics &) {}
  virtual void resized() { layout.performLayout(getLocalBounds()); }

protected:
  juce::Grid layout;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LayoutComponent)
};
