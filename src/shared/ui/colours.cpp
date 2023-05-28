#include "colours.hpp"

#include "../constants.hpp"

namespace colours {

juce::Colour getFilterColour(int filterID) {
  jassert(filterID >= 1 && filterID <= 12);

  switch (filterID) {
  case 1:
    return juce::Colour(0xff279d87);
  case 2:
    return juce::Colour(0xff237fb3);
  case 3:
    return juce::Colour(0xfffab935);
  case 4:
    return juce::Colour(0xffa1976a);
  case 5:
    return juce::Colour(0xff7e3e6f);
  case 6:
    return juce::Colour(0xffc79d52);
  case 7:
    return juce::Colour(0xff7e618a);
  case 8:
    return juce::Colour(0xffee72f3);
  case 9:
    return juce::Colour(0xff89dfe8);
  case 10:
    return juce::Colour(0xff5423ac);
  case 11:
    return juce::Colour(0xff97d385);
  case 12:
    return juce::Colour(0xff687cdc);
  }
}

} // namespace colours
