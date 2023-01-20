#include "plot.hpp"

AnalyzerPlot::GridPlot::GridPlot() {
  for (int order = 1; order <= 5; order++) {
    for (int i = 2; i <= (order == 5 ? 2 : 10); i++) {
      int freq = i * pow(10, order);

      xLogFrequencies.insert(std::pair<int, float>(
          freq, juce::mapFromLog10((float)freq, 20.f, 20000.f)));
    }
  }
}

void AnalyzerPlot::GridPlot::paint(juce::Graphics &g) {
  g.setColour(shared::ui::Colours::grid);

  float width = getWidth();
  float height = getHeight();

  for (auto const &[freq, normFreq] : xLogFrequencies) {
    auto x = getWidth() * normFreq;

    g.drawVerticalLine((int)x, 0, height);
  }

  for (int i = 0; i <= 4; i++) {
    g.drawHorizontalLine(height * (1 - (float)i / 4), 0, width);
  }

  auto box = juce::Rectangle(0, 0, (int)width, (int)height);
  g.drawRect(box);
}

AnalyzerPlot::AxisXPlot::AxisXPlot() {
  for (int order = 1; order <= 5; order++) {
    for (int i : {2, 5, 10}) {
      int freq = i * pow(10, order);

      if (freq >= 1000) {
        labelFrequencies.insert(std::pair<int, juce::String>(
            freq, juce::String(freq / 1000) + "K"));
      } else {
        labelFrequencies.insert(std::pair<int, juce::String>(freq, freq));
      }
    }
  }
}

void AnalyzerPlot::AxisXPlot::paint(juce::Graphics &g) {
  g.setColour(shared::ui::Colours::secondary);

  for (auto const &[freq, stringFreq] : labelFrequencies) {
    auto normFreq = juce::mapFromLog10((float)freq, 20.f, 20000.f);

    auto x = getWidth() * normFreq;

    g.drawText(stringFreq, x, 0, getWidth(), 1, juce::Justification::top);
  }
}

void AnalyzerPlot::AxisYPlot::paint(juce::Graphics &g) {
  g.setColour(shared::ui::Colours::secondary);

  float height = getHeight();
  float width = getWidth();

  for (int i = 0; i < 4; i++) {
    auto y = height * (1 - (float)i / 4);
    auto r = juce::Rectangle(juce::Point<float>(width - 50, y),
                             juce::Point<float>(width, y - 50));

    g.drawText(juce::String(-100 + 25 * i) + " dB", r,
               juce::Justification::bottomLeft);
  }
}

AnalyzerPlot::AnalyzerPlot(shared::lib::BaseProcessor &audioProcessor)
    : BaseComponent(), audioProcessor(audioProcessor),
      spectrumPlot(audioProcessor) {
  addAndMakeVisible(gridPlot);
  addAndMakeVisible(axisXPlot);
  addAndMakeVisible(axisYPlot);
  addAndMakeVisible(spectrumPlot);

  grid.templateRows = {Track(Fr(1)), Track(Px(8)), Track(Px(50))};
  grid.templateColumns = {Track(Fr(1)), Track(Px(8)), Track(Px(50))};
  grid.items = {
      juce::GridItem(gridPlot),  juce::GridItem(), juce::GridItem(axisYPlot),
      juce::GridItem(),          juce::GridItem(), juce::GridItem(),
      juce::GridItem(axisXPlot), juce::GridItem(), juce::GridItem()};

  spectrumGrid.templateRows = {Track(Fr(1)), Track(Px(8)), Track(Px(50))};
  spectrumGrid.templateColumns = {Track(Fr(1)), Track(Px(8)), Track(Px(50))};
  spectrumGrid.items = {juce::GridItem(spectrumPlot),
                        juce::GridItem(),
                        juce::GridItem(),
                        juce::GridItem(),
                        juce::GridItem(),
                        juce::GridItem(),
                        juce::GridItem(),
                        juce::GridItem(),
                        juce::GridItem()};

  resized();
}

void AnalyzerPlot::resized() {
  grid.performLayout(getLocalBounds());
  spectrumGrid.performLayout(getLocalBounds());
}