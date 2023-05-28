#include "filter_panel.hpp"

#include <BinaryData.h>

#include <cmath>
#include <numbers>

FilterPanel::ParametersWrapper::ParametersWrapper(int id, PluginProcessor &p)
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

  typeComboBox.setColour(juce::ComboBox::backgroundColourId,
                         juce::Colours::transparentWhite);
  typeComboBox.setColour(juce::ComboBox::arrowColourId,
                         juce::Colour(0xff233248).darker(0.3f));
  typeComboBox.setColour(juce::ComboBox::textColourId,
                         juce::Colour(0xff233248).darker(0.3f));
  typeComboBox.addItem("Lowpass", 1);
  typeComboBox.addItem("Peak", 2);
  typeComboBox.addItem("Highpass", 3);
  typeComboBox.setSelectedId(1);
  typeComboBoxAttachment = std::make_unique<types::APVTS::ComboBoxAttachment>(
      pluginProcessor.getAPVTS(), FilterParameters::getFilterTypeChoiceID(id),
      typeComboBox);
  // addAndMakeVisible(typeComboBox);

  activeFilterButton.setButtonText("asd");
  activeFilterButtonAttachment =
      std::make_unique<types::APVTS::ButtonAttachment>(
          pluginProcessor.getAPVTS(), FilterParameters::getIsActiveID(id),
          activeFilterButton);
  // addAndMakeVisible(activeFilterButton);

  lowpassButton.onClick = [&]() {
    FilterParameters params(filterID, pluginProcessor.getAPVTS());
    params.filterTypeChoice->beginChangeGesture();
    params.filterTypeChoice->setValueNotifyingHost(0.0f);
    params.filterTypeChoice->endChangeGesture();
  };

  peakButton.onClick = [&]() {
    FilterParameters params(filterID, pluginProcessor.getAPVTS());
    params.filterTypeChoice->beginChangeGesture();
    params.filterTypeChoice->setValueNotifyingHost(0.5f);
    params.filterTypeChoice->endChangeGesture();
  };

  highpassButton.onClick = [&]() {
    FilterParameters params(filterID, pluginProcessor.getAPVTS());
    params.filterTypeChoice->beginChangeGesture();
    params.filterTypeChoice->setValueNotifyingHost(1.0f);
    params.filterTypeChoice->endChangeGesture();
  };

  addAndMakeVisible(lowpassButton);
  addAndMakeVisible(peakButton);
  addAndMakeVisible(highpassButton);

  resized();
}

void FilterPanel::ParametersWrapper::resized() {
  layout.templateRows = {Track(Px(40)), Track(Fr(1))};
  layout.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

  // layout.items.add(juce::GridItem(typeComboBox).withArea(1, 1, 1, 1));
  // layout.items.add(juce::GridItem(activeFilterButton).withArea(1, 3, 1, 3));
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
  prevFilterButton.setShape(createPrevTrianglePath(), true, false, false);
  prevFilterButton.onClick = [&]() {
    uiState.selectPrevFilter();
    updateEqualizerCallback();
  };
  addAndMakeVisible(prevFilterButton);

  nextFilterButton.setShape(createNextTrianglePath(), true, false, false);
  nextFilterButton.onClick = [&]() {
    uiState.selectNextFilter();
    updateEqualizerCallback();
  };
  addAndMakeVisible(nextFilterButton);

  filterLabel.setFont(juce::Font(24.0f));
  filterLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(filterLabel);

  for (int i = constants::FILTER_MIN_ID; i <= constants::FILTER_MAX_ID; ++i) {
    parameterWrappers.push_back(std::make_unique<ParametersWrapper>(i, p));
    addChildComponent(*parameterWrappers.back());
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
  for (auto &parametersWrapper : parameterWrappers) {
    layout.items.add(juce::GridItem(*parametersWrapper)
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
    parameterWrappers[static_cast<size_t>(id) - 1]->setVisible(shouldBeVisible);
  }

  setVisible(true);
}

juce::Path FilterPanel::createNextTrianglePath() {
  const float pi = static_cast<float>(std::numbers::pi);
  const float firstAngle = 2.0f * pi / 3.0f;
  const float secondAngle = 2.0f * firstAngle;

  juce::Path p;
  p.startNewSubPath(1.0f, 0.0f);
  p.lineTo(std::cos(firstAngle), std::sin(firstAngle));
  p.lineTo(std::cos(secondAngle), std::sin(secondAngle));
  p.closeSubPath();
  p = p.createPathWithRoundedCorners(0.333f);

  return p;
}

juce::Path FilterPanel::createPrevTrianglePath() {
  const float pi = static_cast<float>(std::numbers::pi);

  juce::Path p(createNextTrianglePath());
  p.applyTransform(juce::AffineTransform::rotation(pi));

  return p;
}