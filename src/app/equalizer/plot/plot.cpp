#include "plot.hpp"

EqualizerPlot::GridPlot::GridPlot() {
  for (int order = 0; order <= 5; order++) {
    for (int i = (order == 0 ? 10 : 2); i <= (order == 5 ? 2 : 10); i++) {
      int freq = i * pow(10, order);

      xLogFrequencies.insert(std::pair<int, float>(
          freq, juce::mapFromLog10((float)freq, 10.f, 20001.f)));
    }
  }
}

void EqualizerPlot::GridPlot::paint(juce::Graphics &g) {
  g.setColour(juce::Colour::fromString("#FFDBDBDB"));

  for (auto const &[freq, normFreq] : xLogFrequencies) {
    auto x = getWidth() * normFreq;

    g.drawVerticalLine((int)x, 0, (float)getHeight());
  }

  auto box = juce::Rectangle(0, 0, getWidth(), getHeight());
  g.drawRect(box);
}

EqualizerPlot::AxisXPlot::AxisXPlot() {
  for (int order = 0; order <= 5; order++) {
    for (int i : {2, 5, 10}) {
      int freq = i * pow(10, order);

      if (freq >= 1000) {
        labelFrequencies.insert(std::pair<int, juce::String>(
            freq, juce::String(freq / 1000) + "k"));
      } else {
        labelFrequencies.insert(std::pair<int, juce::String>(freq, freq));
      }
    }
  }
}

void EqualizerPlot::AxisXPlot::paint(juce::Graphics &g) {
  g.setColour(juce::Colour::fromString("#FF363636"));

  for (auto const &[freq, stringFreq] : labelFrequencies) {
    auto normFreq = juce::mapFromLog10((float)freq, 10.f, 20001.f);

    auto x = getWidth() * normFreq;

    g.drawText(stringFreq, x, 0, getWidth(), 1, juce::Justification::top);
  }
}

EqualizerPlot::EqualizerPlot(BaseProcessor &audioProcessor)
    : BaseComponent(), audioProcessor(audioProcessor),
      spectrumPlot(audioProcessor) {
  addAndMakeVisible(gridPlot);
  addAndMakeVisible(axisXPlot);
  addAndMakeVisible(spectrumPlot);

  grid.templateRows = {Track(Fr(1)), Track(Px(8)), Track(Px(16))};
  grid.templateColumns = {Track(Fr(1))};
  grid.items = {juce::GridItem(gridPlot), juce::GridItem(),
                juce::GridItem(axisXPlot)};

  spectrumGrid.templateRows = {Track(Fr(1)), Track(Px(24))};
  spectrumGrid.templateColumns = {Track(Fr(1))};
  spectrumGrid.items = {juce::GridItem(spectrumPlot)};

  resized();
}

void EqualizerPlot::resized() {
  grid.performLayout(getLocalBounds());
  // spectrumPlot.setBounds(getLocalBounds());
  spectrumGrid.performLayout(getLocalBounds());
}