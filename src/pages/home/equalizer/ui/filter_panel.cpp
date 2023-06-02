#include "filter_panel.hpp"

#include <BinaryData.h>

#include <cmath>
#include <numbers>

FilterPanel::Wrapper::Wrapper(int id, PluginProcessor &p)
    : filterID(id), pluginProcessor(p), qualitySlider("Q"),
      frequencySlider("F"), gainSlider("G"),
      lowpassButton("lowpass", BinaryData::lowpass_svg),
      peakButton("peak", BinaryData::bell_svg),
      highpassButton("highpass", BinaryData::highpass_svg) {
  qualitySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  qualitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 15);
  qualitySlider.setRange(0.0, 1.0, 0.01);
  qualitySliderAttachment = std::make_unique<types::APVTS::SliderAttachment>(
      pluginProcessor.getAPVTS(), FilterParameters::getQualityID(id),
      qualitySlider);
  addAndMakeVisible(qualitySlider);

  frequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  frequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
  frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 15);
  frequencySlider.setRange(0.0, 1.0, 0.01);
  frequencySlider.setTextValueSuffix(" Hz");
  frequencySliderAttachment = std::make_unique<types::APVTS::SliderAttachment>(
      pluginProcessor.getAPVTS(), FilterParameters::getFrequencyID(id),
      frequencySlider);
  addAndMakeVisible(frequencySlider);

  gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
  gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 15);
  gainSlider.setRange(0.0, 1.0, 0.01);
  gainSlider.setTextValueSuffix(" dB");
  gainSliderAttachment = std::make_unique<types::APVTS::SliderAttachment>(
      pluginProcessor.getAPVTS(), FilterParameters::getGainID(id), gainSlider);
  addAndMakeVisible(gainSlider);

  lowpassButton.onClick = [&]() {
    FilterParameters params(filterID, pluginProcessor.getAPVTS());
    params.filterTypeChoice->beginChangeGesture();
    params.filterTypeChoice->setValueNotifyingHost(0.0f);
    params.filterTypeChoice->endChangeGesture();
  };
  addAndMakeVisible(lowpassButton);

  peakButton.onClick = [&]() {
    FilterParameters params(filterID, pluginProcessor.getAPVTS());
    params.filterTypeChoice->beginChangeGesture();
    params.filterTypeChoice->setValueNotifyingHost(0.5f);
    params.filterTypeChoice->endChangeGesture();
  };
  addAndMakeVisible(peakButton);

  highpassButton.onClick = [&]() {
    FilterParameters params(filterID, pluginProcessor.getAPVTS());
    params.filterTypeChoice->beginChangeGesture();
    params.filterTypeChoice->setValueNotifyingHost(1.0f);
    params.filterTypeChoice->endChangeGesture();
  };
  addAndMakeVisible(highpassButton);

  resized();
}

void FilterPanel::Wrapper::resized() {
  layout.templateRows = {Track(Px(40)), Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

  layout.items.add(juce::GridItem(lowpassButton).withArea(1, 1, 1, 1));
  layout.items.add(juce::GridItem(peakButton).withArea(1, 2, 1, 2));
  layout.items.add(juce::GridItem(highpassButton).withArea(1, 3, 1, 3));
  layout.items.add(juce::GridItem(qualitySlider).withArea(2, 1, 2, 1));
  layout.items.add(juce::GridItem(frequencySlider).withArea(2, 2, 2, 2));
  layout.items.add(juce::GridItem(gainSlider).withArea(2, 3, 2, 3));

  LayoutComponent::resized();
}

FilterPanel::FilterPanel(PluginProcessor &p, std::function<void()> uec)
    : pluginProcessor(p), uiState(p.getUiState()), updateEqualizerCallback(uec),
      prevFilterButton("prev_filter", juce::Colour(0xff233248).darker(0.3f),
                       colours::primaryColour, colours::responseCurveColour),
      nextFilterButton("next_filter", juce::Colour(0xff233248).darker(0.3f),
                       colours::primaryColour, colours::responseCurveColour) {
  prevFilterButton.setShape(paths::createRoundedTriangle(180.0f), true, false,
                            false);
  prevFilterButton.onClick = [&]() {
    uiState.selectPrevFilter();
    updateEqualizerCallback();
  };
  addAndMakeVisible(prevFilterButton);

  nextFilterButton.setShape(paths::createRoundedTriangle(0.0f), true, false,
                            false);
  nextFilterButton.onClick = [&]() {
    uiState.selectNextFilter();
    updateEqualizerCallback();
  };
  addAndMakeVisible(nextFilterButton);

  filterLabel.setFont(juce::Font(24.0f));
  filterLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(filterLabel);

  for (int i = constants::FILTER_MIN_ID; i <= constants::FILTER_MAX_ID; ++i) {
    wrappers.push_back(std::make_unique<Wrapper>(i, p));
    addChildComponent(*wrappers.back());
  }

  resized();
}

void FilterPanel::paint(juce::Graphics &g) {
  juce::Path background;
  background.addRoundedRectangle(getLocalBounds(), cornerSize);
  g.setColour(juce::Colours::white);
  g.fillPath(background);

  g.setColour(juce::Colour(0xff233248).darker(0.3f));
  g.drawRoundedRectangle(
      getLocalBounds().toFloat().reduced(0.5f * lineThickness), cornerSize,
      lineThickness);
}

void FilterPanel::resized() {
  layout.templateRows = {Track(Px(paddingTop + lineThickness)),
                         Track(Px(headerHeight)), Track(Fr(1)),
                         Track(Px(paddingBottom + lineThickness))};
  layout.templateColumns = {
      Track(Px(paddingLeft + lineThickness)),
      Track(Px(buttonSize)),
      Track(Fr(1)),
      Track(Px(labelSize)),
      Track(Fr(1)),
      Track(Px(buttonSize)),
      Track(Px(paddingRight + lineThickness)),
  };

  layout.items.add(juce::GridItem(prevFilterButton)
                       .withArea(2, 2, 2, 2)
                       .withSize(buttonSize, buttonSize)
                       .withJustifySelf(juce::GridItem::JustifySelf::center)
                       .withAlignSelf(juce::GridItem::AlignSelf::center));
  layout.items.add(juce::GridItem(filterLabel).withArea(2, 4, 2, 4));
  layout.items.add(juce::GridItem(nextFilterButton)
                       .withArea(2, 6, 2, 6)
                       .withSize(buttonSize, buttonSize)
                       .withJustifySelf(juce::GridItem::JustifySelf::center)
                       .withAlignSelf(juce::GridItem::AlignSelf::center));
  for (auto &wrapper : wrappers) {
    layout.items.add(juce::GridItem(*wrapper)
                         .withArea(3, 3, 3, 6)
                         .withJustifySelf(juce::GridItem::JustifySelf::end)
                         .withAlignSelf(juce::GridItem::AlignSelf::end));
  }

  constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(),
                                        getWidth());

  LayoutComponent::resized();
}

void FilterPanel::update() {
  if (!uiState.hasSelectedFilter()) {
    setVisible(false);
    return;
  }

  filterLabel.setText(
      juce::String::formatted("Filter %d", uiState.selectedFilterID),
      juce::NotificationType::dontSendNotification);
  filterLabel.setColour(juce::Label::textColourId,
                        colours::getFilterColour(uiState.selectedFilterID));

  for (const auto &[id, isUsed] : uiState.usedFilterIDs) {
    bool shouldBeVisible = (id == uiState.selectedFilterID);
    wrappers[static_cast<size_t>(id) - 1]->setVisible(shouldBeVisible);
  }

  setVisible(true);
}