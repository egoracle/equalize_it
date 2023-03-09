#include "grid.hpp"

Grid::Grid() : juce::Component() {
  for (int order = 1; order <= 5; order++) {
    for (int i = 2; i < (order == 5 ? 2 : 10); i++) {
      int freq = i * int(pow(10, order));

      logScaledFrequencies.insert(std::pair<int, float>(
          freq, juce::mapFromLog10((float)freq, 20.f, 20000.f)));
    }
  }
}

Grid::~Grid() {}

void Grid::paint(juce::Graphics &g) {
  g.setColour(juce::Colour(0xfff2f2f2));

  float width = float(getWidth());
  float height = float(getHeight());

  for (auto const &[freq, logFreq] : logScaledFrequencies) {
    float x = width * logFreq;

    g.drawVerticalLine(int(x), 0.0f, height);
  }

  for (int i = 0; i < 4; i++) {
    int y = int(height * (1 - float(i) / 4));

    g.drawHorizontalLine(y, 0, width);
  }
}

void Grid::resized() {}