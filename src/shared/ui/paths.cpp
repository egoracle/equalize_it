#include "paths.hpp"

#include <cmath>
#include <numbers>

namespace paths {

juce::Path createRoundedTriangle(float angle) {
  const float pi = static_cast<float>(std::numbers::pi);
  const float firstAngle = 2.0f * pi / 3.0f;
  const float secondAngle = 2.0f * firstAngle;

  juce::Path triangle;
  triangle.startNewSubPath(1.0f, 0.0f);
  triangle.lineTo(std::cos(firstAngle), std::sin(firstAngle));
  triangle.lineTo(std::cos(secondAngle), std::sin(secondAngle));
  triangle.closeSubPath();

  triangle.applyTransform(juce::AffineTransform::rotation(angle * pi / 180.0f));

  return triangle.createPathWithRoundedCorners(0.333f);
}

juce::Path createCircle() {
  juce::Path circle;

  circle.addEllipse(0.0f, 0.0f, 1.0f, 1.0f);

  return circle;
}

} // namespace paths
