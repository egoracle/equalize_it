#pragma once

#include "layout_component.hpp"

#include <BinaryData.h>
#include <friz/friz.h>

class IconButton : public juce::DrawableButton {
public:
  class LookAndFeel : public juce::LookAndFeel_V4 {
  public:
    LookAndFeel();

    void drawButtonBackground(juce::Graphics &, juce::Button &,
                              const juce::Colour &, bool, bool) override;

    void setBackgroundColour(const juce::Colour &);

  private:
    juce::Colour backgroundColour;
  };

  class AnimatorProperties {
  public:
    const juce::uint32 toBackgoundColorInt = 0xffe3e4e6;
    static constexpr float animationDuration = 200.0f;
    static constexpr float fromAlpha = 0.0f;
    static constexpr float toAlpha = 0.3f;
    const static friz::Parametric::CurveType parametricCurveType =
        friz::Parametric::kEaseInCubic;

  public:
    AnimatorProperties(IconButton *);

    void addFromEffect(friz::Animator &);
    void addToEffect(friz::Animator &);

  private:
    IconButton *button;

    juce::uint8 r, g, b;

    int fromAnimationId = 0;
    int toAnimationId = 1;
  };

public:
  IconButton(const juce::String &, const char *);
  ~IconButton() override;

  void mouseEnter(const juce::MouseEvent &) override;
  void mouseExit(const juce::MouseEvent &) override;

private:
  LookAndFeel lookAndFeel;

  AnimatorProperties animatorProps;
  friz::Animator animator;
};