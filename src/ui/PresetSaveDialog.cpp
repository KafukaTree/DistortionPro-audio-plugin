/**
 * PresetSaveDialog.cpp
 *
 * Dialog for saving user presets
 */

#include "PresetSaveDialog.h"

namespace DistortionPro {

PresetSaveDialog::PresetSaveDialog(juce::AudioProcessorValueTreeState& valueTree)
    : DialogWindow("Save Preset", juce::Colours::darkgrey, true),
      valueTree_(valueTree)
{
    setupUI();

    setContentOwned(&nameEditor_, true);
    setResizable(false, false);
    centreWithSize(320, 120);
}

PresetSaveDialog::~PresetSaveDialog() {
}

void PresetSaveDialog::setupUI() {
    // Create a component to hold our controls
    auto* content = new juce::Component();
    content->setSize(300, 100);

    nameLabel_.setText("Preset Name:", juce::dontSendNotification);
    nameLabel_.setBounds(10, 10, 100, 25);
    nameLabel_.setColour(juce::Label::textColourId, juce::Colours::white);
    content->addAndMakeVisible(nameLabel_);

    nameEditor_.setBounds(10, 35, 280, 30);
    nameEditor_.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    nameEditor_.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    nameEditor_.setText("My Preset");
    content->addAndMakeVisible(nameEditor_);

    saveButton_.setButtonText("Save");
    saveButton_.setBounds(10, 70, 80, 25);
    saveButton_.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    saveButton_.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    saveButton_.onClick = [this] { savePreset(); };
    content->addAndMakeVisible(saveButton_);

    cancelButton_.setButtonText("Cancel");
    cancelButton_.setBounds(100, 70, 80, 25);
    cancelButton_.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    cancelButton_.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    cancelButton_.onClick = [this] { closeButtonPressed(); };
    content->addAndMakeVisible(cancelButton_);

    setContentOwned(content, true);
}

void PresetSaveDialog::savePreset() {
    juce::String name = nameEditor_.getText().trim();

    if (name.isNotEmpty()) {
        if (onPresetSaved) {
            onPresetSaved(name);
        }
        closeButtonPressed();
    } else {
        nameEditor_.setColour(juce::TextEditor::outlineColourId, juce::Colours::red);
        nameEditor_.repaint();
    }
}

void PresetSaveDialog::closeButtonPressed() {
    exitModalState(0);
}

}  // namespace DistortionPro
