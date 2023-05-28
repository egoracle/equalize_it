#include "slider.hpp"

#include <cmath>
#include <numbers>

#include "colours.hpp"

void EqItSlider::LookAndFeel::drawRotarySlider(
    juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) {
  const float radius = juce::jmin(width / 2, height / 2) * 0.75f;
  const float centreX = static_cast<float>(x + width) * 0.5f;
  const float centreY = static_cast<float>(y + height) * 0.5f;
  const float rx = centreX - radius;
  const float ry = centreY - radius;
  const float rw = radius * 2.0f;
  const float angle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

  g.setColour(juce::Colour(0xff233248).darker(0.3f));
  juce::Path filledArc;
  filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle,
                          0.75f);

  g.fillPath(filledArc);

  const auto bounds = slider.getLocalBounds();

  g.setFont(25.0f);
  g.drawText(
      dynamic_cast<EqItSlider &>(slider).getLabel(),
      bounds.translated(-bounds.getCentreX(), -bounds.getCentreY())
          .translated(static_cast<int>(centreX), static_cast<int>(centreY)),
      juce::Justification::centred);

  const float pi = static_cast<float>(std::numbers::pi);
  const float firstAngle = 2.0f * pi / 3.0f;
  const float secondAngle = 2.0f * firstAngle;

  juce::AffineTransform transform(juce::AffineTransform::scale(8.0f)
                                      .rotated(pi * 0.5f)
                                      .translated(0.0f, -radius)
                                      .rotated(angle)
                                      .translated(centreX, centreY));

  juce::Path triangle;
  triangle.startNewSubPath(1.0f, 0.0f);
  triangle.lineTo(std::cos(firstAngle), std::sin(firstAngle));
  triangle.lineTo(std::cos(secondAngle), std::sin(secondAngle));
  triangle.closeSubPath();
  triangle = triangle.createPathWithRoundedCorners(0.333f);
  triangle.applyTransform(transform);

  juce::Path circle;
  circle.addEllipse(-1.0f, -1.0f, 2.0f, 2.0f);
  circle.applyTransform(transform);

  juce::Path triangle2;
  triangle2.startNewSubPath(1.0f, 0.0f);
  triangle2.lineTo(std::cos(firstAngle), std::sin(firstAngle));
  triangle2.lineTo(std::cos(secondAngle), std::sin(secondAngle));
  triangle2.closeSubPath();
  triangle2 = triangle2.createPathWithRoundedCorners(0.333f);
  triangle2.applyTransform(juce::AffineTransform::scale(12.0f)
                               .rotated(pi * 0.5f)
                               .translated(0.0f, -radius)
                               .rotated(angle)
                               .translated(centreX, centreY));

  g.setColour(juce::Colours::white);
  g.fillPath(triangle2);

  g.setColour(juce::Colour(0xff2683ee).brighter(0.333f));
  g.fillPath(triangle);
}

juce::Label *
EqItSlider::LookAndFeel::createSliderTextBox(juce::Slider &slider) {
  juce::Label *label = juce::LookAndFeel_V4::createSliderTextBox(slider);
  label->setColour(juce::Label::backgroundColourId,
                   juce::Colours::transparentWhite);
  label->setColour(juce::Label::textColourId, juce::Colours::black);
  label->setColour(juce::Label::backgroundWhenEditingColourId,
                   juce::Colours::transparentWhite);
  label->setColour(juce::Label::textWhenEditingColourId, juce::Colours::black);
  label->setColour(juce::Label::outlineColourId,
                   juce::Colours::transparentWhite);
  label->setColour(juce::Label::outlineWhenEditingColourId,
                   juce::Colours::transparentWhite);

  return label;
}

EqItSlider::EqItSlider(juce::String l) : label(l) {
  setLookAndFeel(&lookAndFeel);
}

EqItSlider::~EqItSlider() { setLookAndFeel(nullptr); }

juce::String &EqItSlider::getLabel() { return label; }
