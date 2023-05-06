#include "logo.hpp"

#include <BinaryData.h>

Logo::Logo()
    : drawable(juce::Drawable::createFromSVG(
          *juce::XmlDocument::parse(BinaryData::logo_svg))) {}

void Logo::paint(juce::Graphics &g) {
  drawable->setTransformToFit(
      juce::Rectangle<int>(0, 0, getWidth(), getHeight()).toFloat(),
      juce::RectanglePlacement::centred);

  drawable->draw(g, 1.0f);
}
