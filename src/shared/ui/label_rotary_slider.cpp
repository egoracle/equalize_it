#include "label_rotary_slider.hpp"

#include <cmath>
#include <numbers>

#include "colours.hpp"
#include "paths.hpp"

LabelRotarySlider::LabelRotarySlider(juce::String l) : label(l) {
  setLookAndFeel(&lookAndFeel);
  setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
}

LabelRotarySlider::LabelRotarySlider() : LabelRotarySlider("") {}

LabelRotarySlider::~LabelRotarySlider() { setLookAndFeel(nullptr); }

void LabelRotarySlider::LookAndFeel::drawRotarySlider(
    juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) {
  const float pi = static_cast<float>(std::numbers::pi);
  const float radius =
      static_cast<float>(juce::jmin(width, height)) * 0.5f * 0.75f;
  const float centreX = static_cast<float>(x + width) * 0.5f;
  const float centreY = static_cast<float>(y + height) * 0.5f;
  const float rx = centreX - radius;
  const float ry = centreY - radius;
  const float rw = radius * 2.0f;
  const float angle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
  const auto bounds = slider.getLocalBounds();

  const auto makeTransform = [&](float scaleFactor) {
    return juce::AffineTransform::scale(scaleFactor)
        .rotated(pi * 0.5f)
        .translated(0.0f, -radius)
        .rotated(angle)
        .translated(centreX, centreY);
  };

  juce::Path filledArc;
  filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle,
                          0.75f);

  juce::Path backTriangle(paths::createRoundedTriangle(0.0f));
  backTriangle.applyTransform(makeTransform(12.0f));

  juce::Path triangle(paths::createRoundedTriangle(0.0f));
  triangle.applyTransform(makeTransform(8.0f));

  g.setColour(juce::Colour(0xff233248).darker(0.3f));
  g.fillPath(filledArc);

  g.setFont(25.0f);
  g.drawText(
      dynamic_cast<LabelRotarySlider &>(slider).getLabel(),
      bounds.translated(-bounds.getCentreX(), -bounds.getCentreY())
          .translated(static_cast<int>(centreX), static_cast<int>(centreY)),
      juce::Justification::centred);

  g.setColour(juce::Colours::white);
  g.fillPath(backTriangle);

  g.setColour(juce::Colour(0xff2683ee).brighter(0.333f));
  g.fillPath(triangle);
}

juce::Label *
LabelRotarySlider::LookAndFeel::createSliderTextBox(juce::Slider &slider) {
  juce::Label *textBox = juce::LookAndFeel_V4::createSliderTextBox(slider);
  textBox->setColour(juce::Label::backgroundColourId,
                     juce::Colours::transparentWhite);
  textBox->setColour(juce::Label::backgroundWhenEditingColourId,
                     juce::Colours::transparentWhite);
  textBox->setColour(juce::Label::textWhenEditingColourId,
                     juce::Colours::black);
  textBox->setColour(juce::Label::outlineColourId,
                     juce::Colours::transparentWhite);
  textBox->setColour(juce::Label::outlineWhenEditingColourId,
                     juce::Colours::transparentWhite);
  textBox->setColour(juce::Label::textColourId, juce::Colours::black);

  return textBox;
}

juce::String &LabelRotarySlider::getLabel() noexcept { return label; }