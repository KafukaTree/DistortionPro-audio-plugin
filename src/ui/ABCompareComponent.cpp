/**
 * ABCompareComponent.cpp
 *
 * A/B comparison component implementation
 */

#include "ABCompareComponent.h"
#include <juce_audio_processors/juce_audio_processors.h>

namespace DistortionPro {

ABCompareComponent::ABCompareComponent(juce::AudioProcessorValueTreeState& valueTree)
    : valueTree_(valueTree)
{
    setupUI();
}

ABCompareComponent::~ABCompareComponent() {
}

void ABCompareComponent::setupUI() {
    compareToggle_.setButtonText("A/B");
    compareToggle_.setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);
    compareToggle_.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    compareToggle_.onClick = [this] {
        enabled_ = compareToggle_.getToggleState();
        if (enabled_) {
            compare();
        }
        if (onCompareChanged) {
            onCompareChanged();
        }
    };
    addAndMakeVisible(compareToggle_);

    labelA_.setText("A", juce::dontSendNotification);
    labelA_.setColour(juce::Label::textColourId, juce::Colours::cyan);
    labelA_.setFont(juce::Font(10.0f, juce::Font::bold));
    addAndMakeVisible(labelA_);

    labelB_.setText("B", juce::dontSendNotification);
    labelB_.setColour(juce::Label::textColourId, juce::Colours::orange);
    labelB_.setFont(juce::Font(10.0f, juce::Font::bold));
    addAndMakeVisible(labelB_);
}

void ABCompareComponent::setEnabled(bool enabled) {
    enabled_ = enabled;
    compareToggle_.setToggleState(enabled, juce::dontSendNotification);
}

void ABCompareComponent::saveCurrentAsA() {
    // Save current parameters to A
    paramsA_.drive = valueTree_.getRawParameterValue("drive")->load();
    paramsA_.tone = valueTree_.getRawParameterValue("tone")->load();
    paramsA_.output = valueTree_.getRawParameterValue("output")->load();
    paramsA_.mix = valueTree_.getRawParameterValue("mix")->load();
    paramsA_.depth = valueTree_.getRawParameterValue("depth")->load();
    paramsA_.attack = valueTree_.getRawParameterValue("attack")->load();
}

void ABCompareComponent::saveCurrentAsB() {
    // Save current parameters to B
    paramsB_.drive = valueTree_.getRawParameterValue("drive")->load();
    paramsB_.tone = valueTree_.getRawParameterValue("tone")->load();
    paramsB_.output = valueTree_.getRawParameterValue("output")->load();
    paramsB_.mix = valueTree_.getRawParameterValue("mix")->load();
    paramsB_.depth = valueTree_.getRawParameterValue("depth")->load();
    paramsB_.attack = valueTree_.getRawParameterValue("attack")->load();
}

void ABCompareComponent::compare() {
    if (!enabled_) {
        return;
    }

    // Toggle between A and B based on modifier key or automatically
    // For simplicity, we alternate based on time or button press
    // This is a simplified implementation
}

void ABCompareComponent::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();

    if (enabled_) {
        g.setColour(juce::Colours::orange.withAlpha(0.2f));
        g.fillRoundedRectangle(bounds, 4.0f);
    }
}

void ABCompareComponent::resized() {
    auto bounds = getLocalBounds();
    int width = bounds.getWidth();

    compareToggle_.setBounds(bounds.getX(), bounds.getY(), width / 3, bounds.getHeight());
    labelA_.setBounds(bounds.getX() + width / 3, bounds.getY(), width / 3, bounds.getHeight());
    labelB_.setBounds(bounds.getX() + width * 2 / 3, bounds.getY(), width / 3, bounds.getHeight());
}

}  // namespace DistortionPro
