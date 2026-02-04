/**
 * KnobComponent.cpp
 *
 * Rotary knob implementation
 */

#include "KnobComponent.h"

namespace DistortionPro {

KnobComponent::KnobComponent(const juce::String& name, juce::AudioProcessorValueTreeState& valueTree,
                              const juce::String& paramId)
    : label_(name)
{
    // Setup slider
    slider_.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider_.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    slider_.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
    slider_.setColour(juce::Slider::trackColourId, juce::Colours::black);

    addAndMakeVisible(slider_);

    // Create attachment
    attachment_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        valueTree, paramId, slider_);

    // Labels
    nameLabel_.setText(name, juce::dontSendNotification);
    nameLabel_.setJustificationType(juce::Justification::centred);
    nameLabel_.setColour(juce::Label::textColourId, juce::Colours::white);
    nameLabel_.setFont(juce::Font(12.0f, juce::Font::plain));
    addAndMakeVisible(nameLabel_);

    valueLabel_.setText("0%", juce::dontSendNotification);
    valueLabel_.setJustificationType(juce::Justification::centred);
    valueLabel_.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    valueLabel_.setFont(juce::Font(11.0f, juce::Font::plain));
    addAndMakeVisible(valueLabel_);

    // Default value display
    valueDisplayFunc_ = [](float value) {
        return juce::String(static_cast<int>(value * 100)) + "%";
    };
}

KnobComponent::~KnobComponent() {
}

void KnobComponent::resized() {
    auto bounds = getLocalBounds();

    // Layout: label on top, knob in middle, value below
    int height = bounds.getHeight();

    nameLabel_.setBounds(bounds.removeFromTop(height / 3));
    valueLabel_.setBounds(bounds.removeFromBottom(height / 4));
    slider_.setBounds(bounds);
}

void KnobComponent::paint(juce::Graphics& g) {
    // Background
    g.setColour(juce::Colours::darkgrey.withAlpha(0.3f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
}

void KnobComponent::sliderValueChanged(juce::Slider* slider) {
    float value = slider->getValue();
    valueLabel_.setText(valueDisplayFunc_(value), juce::dontSendNotification);
}

void KnobComponent::setLabel(const juce::String& newLabel) {
    label_ = newLabel;
    nameLabel_.setText(newLabel, juce::dontSendNotification);
}

void KnobComponent::setValueDisplayFunction(std::function<juce::String(float)> func) {
    valueDisplayFunc_ = func;
}

}  // namespace DistortionPro
