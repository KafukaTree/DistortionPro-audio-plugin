/**
 * TypeSelector.cpp
 *
 * Distortion type selector implementation
 */

#include "TypeSelector.h"

namespace DistortionPro {

TypeSelector::TypeSelector(juce::AudioProcessorValueTreeState& valueTree) {
    static const juce::String paramTypeId = "type";

    // Label
    label_.setText("Type", juce::dontSendNotification);
    label_.setJustificationType(juce::Justification::centredLeft);
    label_.setColour(juce::Label::textColourId, juce::Colours::white);
    label_.setFont(juce::Font(12.0f, juce::Font::plain));
    addAndMakeVisible(label_);

    // ComboBox
    comboBox_.setEditableText(false);
    comboBox_.setJustificationType(juce::Justification::centredLeft);
    comboBox_.setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
    comboBox_.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    comboBox_.setColour(juce::ComboBox::arrowColourId, juce::Colours::white);
    comboBox_.addListener(this);

    populateComboBox();

    addAndMakeVisible(comboBox_);

    // Create attachment
    attachment_ = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        valueTree, paramTypeId, comboBox_);
}

TypeSelector::~TypeSelector() {
}

void TypeSelector::populateComboBox() {
    comboBox_.addItem("Overdrive", 1);
    comboBox_.addItem("Distortion", 2);
    comboBox_.addItem("Fuzz", 3);
    comboBox_.addItem("Saturation", 4);

    comboBox_.setSelectedId(1);  // Default to Overdrive
}

void TypeSelector::resized() {
    auto bounds = getLocalBounds();

    label_.setBounds(bounds.removeFromLeft(50));
    comboBox_.setBounds(bounds);
}

void TypeSelector::paint(juce::Graphics& g) {
    // Background
    g.setColour(juce::Colours::darkgrey.withAlpha(0.3f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
}

void TypeSelector::comboBoxChanged(juce::ComboBox* comboBox) {
    // Handler for combo box changes if needed
}

DistortionType TypeSelector::getSelectedType() const {
    int selectedId = comboBox_.getSelectedId();

    switch (selectedId) {
        case 1:  return DistortionType::Overdrive;
        case 2:  return DistortionType::Distortion;
        case 3:  return DistortionType::Fuzz;
        case 4:  return DistortionType::Saturation;
        default: return DistortionType::Overdrive;
    }
}

void TypeSelector::setSelectedType(DistortionType type) {
    int id;

    switch (type) {
        case DistortionType::Overdrive:   id = 1; break;
        case DistortionType::Distortion:  id = 2; break;
        case DistortionType::Fuzz:        id = 3; break;
        case DistortionType::Saturation:  id = 4; break;
        default:                          id = 1; break;
    }

    comboBox_.setSelectedId(id, juce::sendNotification);
}

}  // namespace DistortionPro
