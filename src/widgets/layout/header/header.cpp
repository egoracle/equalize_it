#include <cmath>

#include "header.hpp"

Header::Header() : LayoutComponent() {
  addAndMakeVisible(logo);
  addAndMakeVisible(rightButtonsStack);

  resized();
}

void Header::paint(juce::Graphics &g) {
  float width = float(getWidth());
  float centerX = width / 2;

  float quarterLogoWidth = logoWidth / 4;

  float height = float(getHeight()) - 1;
  float bodyHeight = height * 0.75f;

  juce::Path p;

  // Painting separator
  p.startNewSubPath(0.0f, bodyHeight);
  p.lineTo(centerX - logoWidth, bodyHeight);
  p.lineTo(centerX - quarterLogoWidth, height);
  p.lineTo(centerX + quarterLogoWidth, height);
  p.lineTo(centerX + logoWidth, bodyHeight);
  p.lineTo(width, bodyHeight);

  g.setColour(juce::Colour(0x60344661));
  g.strokePath(p, juce::PathStrokeType(1));

  // Painting background
  p.lineTo(width, 0.0f);
  p.lineTo(0.0f, 0.0f);
  p.lineTo(0.0f, bodyHeight);

  juce::FillType fill;
  fill.setColour(juce::Colours::white);

  g.setFillType(juce::FillType(juce::Colours::white));
  g.fillPath(p);
}

void Header::resized() {
  float paddingSize =
      std::max<float>((float(getWidth()) - containerWidth) / 2, 0);

  auto contentMargin =
      juce::GridItem::Margin(0.0f, 0.0f, float(getHeight()) * 0.25f, 0.0f);

  layout.templateRows = {Track(Fr(1))};
  layout.templateColumns = {Track(Px(paddingSize)), Track(Px(125)),
                            Track(Fr(1)),           Track(Px(logoWidth)),
                            Track(Fr(1)),           Track(Px(125)),
                            Track(Px(paddingSize))};
  layout.alignItems = juce::Grid::AlignItems::center;
  layout.justifyContent = juce::Grid::JustifyContent::center;
  layout.items = {juce::GridItem(),
                  juce::GridItem(),
                  juce::GridItem(),
                  juce::GridItem(logo),
                  juce::GridItem(),
                  juce::GridItem(rightButtonsStack).withMargin(contentMargin),
                  juce::GridItem()};

  LayoutComponent::resized();
}