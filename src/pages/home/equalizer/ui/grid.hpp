#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <map>

class Grid : public juce::Component {
public:
  Grid();
  ~Grid() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  std::map<int, float> logScaledFrequencies;
};