#include "icon_button.hpp"

IconButton::LookAndFeel::LookAndFeel()
    : juce::LookAndFeel_V4(),
      backgroundColour(juce::Colours::transparentWhite) {}

void IconButton::LookAndFeel::drawButtonBackground(juce::Graphics &g,
                                                   juce::Button &button,
                                                   const juce::Colour &, bool,
                                                   bool) {
  g.setColour(backgroundColour);
  g.fillRect(0, 0, button.getWidth(), button.getHeight());
}

void IconButton::LookAndFeel::setBackgroundColour(const juce::Colour &colour) {
  backgroundColour = colour;
}

IconButton::AnimatorProperties::AnimatorProperties(IconButton *but)
    : button(but) {
  juce::Colour toBackgroundColour(toBackgoundColorInt);
  r = toBackgroundColour.getRed();
  g = toBackgroundColour.getGreen();
  b = toBackgroundColour.getBlue();
}

void IconButton::AnimatorProperties::addFromEffect(friz::Animator &anim) {
  if (anim.getAnimation(fromAnimationId) != nullptr) {
    return;
  }

  auto fromEffect = friz::makeAnimation<friz::Parametric, 1>(
      fromAnimationId, {fromAlpha}, {toAlpha}, animationDuration,
      parametricCurveType);
  fromEffect->updateFn = [this](int, const auto &vals) {
    button->lookAndFeel.setBackgroundColour(juce::Colour(r, g, b, vals[0]));
    button->repaint();
  };

  anim.addAnimation(std::move(fromEffect));
}

void IconButton::AnimatorProperties::addToEffect(friz::Animator &anim) {
  if (anim.getAnimation(toAnimationId) != nullptr) {
    return;
  }

  auto toEffect = friz::makeAnimation<friz::Parametric, 1>(
      fromAnimationId, {toAlpha}, {fromAlpha}, animationDuration,
      parametricCurveType);
  toEffect->updateFn = [this](int, const auto &vals) {
    button->lookAndFeel.setBackgroundColour(juce::Colour(r, g, b, vals[0]));
    button->repaint();
  };

  anim.addAnimation(std::move(toEffect));
}

IconButton::IconButton(const juce::String &name, const char *svgIcon)
    : juce::DrawableButton(
          name, juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
      animatorProps(this), animator{
                               std::make_unique<friz::DisplaySyncController>(
                                   this)} {
  auto drawable =
      juce::Drawable::createFromSVG(*juce::XmlDocument::parse(svgIcon));
  setImages(drawable.get());

  setLookAndFeel(&lookAndFeel);
  setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

IconButton::~IconButton() { setLookAndFeel(nullptr); }

void IconButton::mouseEnter(const juce::MouseEvent &) {
  animatorProps.addFromEffect(animator);
}

void IconButton::mouseExit(const juce::MouseEvent &) {
  animatorProps.addToEffect(animator);
}