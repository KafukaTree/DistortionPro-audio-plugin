/**
 * PluginEditor.cpp
 *
 * Main plugin editor implementation with high-DPI and responsive layout
 */

#include "PluginEditor.h"

namespace DistortionPro {

PluginEditor::PluginEditor(DistortionPro& processor)
    : AudioProcessorEditor(processor),
      processor_(processor),
      valueTree_(processor.getValueTreeState())
{
    // Set minimum size constraint
    setResizeLimits(minWidth, minHeight, 2000, 2000);

    // Get scale factor for high-DPI displays
    updateScaleFactor();

    setupUI();
    setupKnobs();
    setupPresets();
    setupOptions();
    setupStatusBar();

    // Initial layout
    resized();
}

PluginEditor::~PluginEditor() {
}

void PluginEditor::updateScaleFactor() {
    scaleFactor_ = getDesktopScaleFactor();

    // Limit scale factor reasonable range
    scaleFactor_ = juce::jlimit(0.5f, 3.0f, scaleFactor_);
}

void PluginEditor::setupUI() {
    // Title
    titleLabel_.setText("DISTORTION PRO", juce::dontSendNotification);
    titleLabel_.setJustificationType(juce::Justification::centred);
    titleLabel_.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel_.setFont(juce::Font(18.0f * scaleFactor_, juce::Font::bold));
    addAndMakeVisible(titleLabel_);

    // Subtitle
    subtitleLabel_.setText("Professional Distortion Plugin", juce::dontSendNotification);
    subtitleLabel_.setJustificationType(juce::Justification::centred);
    subtitleLabel_.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    subtitleLabel_.setFont(juce::Font(11.0f * scaleFactor_, juce::Font::plain));
    addAndMakeVisible(subtitleLabel_);

    // Waveform displays
    inputWaveform_.setMode(WaveformDisplay::Mode::Input);
    inputWaveform_.setColor(juce::Colours::cyan);
    addAndMakeVisible(inputWaveform_);

    outputWaveform_.setMode(WaveformDisplay::Mode::Output);
    outputWaveform_.setColor(juce::Colours::orange);
    addAndMakeVisible(outputWaveform_);

    // Gain meter
    addAndMakeVisible(gainMeter_);

    // Type selector
    typeSelector_ = new TypeSelector(valueTree_);
    addAndMakeVisible(typeSelector_);
}

void PluginEditor::setupKnobs() {
    // Drive knob
    driveKnob_ = new KnobComponent("Drive", valueTree_, "drive");
    addAndMakeVisible(driveKnob_);

    // Tone knob
    toneKnob_ = new KnobComponent("Tone", valueTree_, "tone");
    addAndMakeVisible(toneKnob_);

    // Output knob
    outputKnob_ = new KnobComponent("Output", valueTree_, "output");
    outputKnob_->setValueDisplayFunction([](float value) {
        float db = (value - 0.75f) * 24.0f;
        return juce::String(static_cast<int>(db)) + " dB";
    });
    addAndMakeVisible(outputKnob_);

    // Mix knob
    mixKnob_ = new KnobComponent("Mix", valueTree_, "mix");
    addAndMakeVisible(mixKnob_);

    // Depth knob
    depthKnob_ = new KnobComponent("Depth", valueTree_, "depth");
    addAndMakeVisible(depthKnob_);

    // Attack knob
    attackKnob_ = new KnobComponent("Attack", valueTree_, "attack");
    addAndMakeVisible(attackKnob_);
}

void PluginEditor::setupPresets() {
    // Preset dropdown - populate from preset manager
    presetCombo_.addItem("Vintage Overdrive", 1);
    presetCombo_.addItem("British Crunch", 2);
    presetCombo_.addItem("Hard Rock", 3);
    presetCombo_.addItem("Fuzzy Math", 4);
    presetCombo_.addItem("Clean Boost", 5);
    presetCombo_.addItem("Studio Warmth", 6);
    presetCombo_.addItem("- User Presets -", 100);
    presetCombo_.setSelectedId(1);
    presetCombo_.setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
    presetCombo_.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    addAndMakeVisible(presetCombo_);

    // Save button
    savePresetButton_.setButtonText("Save");
    savePresetButton_.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    savePresetButton_.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(savePresetButton_);

    // Load button
    loadPresetButton_.setButtonText("Load");
    loadPresetButton_.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    loadPresetButton_.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(loadPresetButton_);

    // A/B compare button
    abCompareButton_.setButtonText("A/B");
    abCompareButton_.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    abCompareButton_.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(abCompareButton_);
}

void PluginEditor::setupOptions() {
    // Oversample toggle
    oversampleToggle_.setButtonText("2x Oversample");
    oversampleToggle_.setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);
    oversampleToggle_.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    addAndMakeVisible(oversampleToggle_);
}

void PluginEditor::setupStatusBar() {
    statusLabel_.setText("Ready", juce::dontSendNotification);
    statusLabel_.setJustificationType(juce::Justification::centredLeft);
    statusLabel_.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    statusLabel_.setFont(juce::Font(11.0f * scaleFactor_, juce::Font::plain));
    addAndMakeVisible(statusLabel_);
}

void PluginEditor::resized() {
    layoutComponents();
}

void PluginEditor::childBoundsChanged(juce::Component* child) {
    // Handle child resize notifications
    (void)child;
}

void PluginEditor::layoutComponents() {
    updateScaleFactor();

    auto bounds = getLocalBounds();

    // Margins scaled by DPI
    int margin = static_cast<int>(10 * scaleFactor_);
    bounds.reduce(margin, margin);

    // Title section
    int titleHeight = static_cast<int>(35 * scaleFactor_);
    auto titleBounds = bounds.removeFromTop(titleHeight);
    titleLabel_.setBounds(titleBounds);
    subtitleLabel_.setBounds(titleBounds.removeFromBottom(static_cast<int>(15 * scaleFactor_)));

    bounds.removeFromTop(static_cast<int>(5 * scaleFactor_));

    // Waveform and meter section
    int waveHeight = static_cast<int>(80 * scaleFactor_);
    auto waveBounds = bounds.removeFromTop(waveHeight);
    int waveWidth = waveBounds.getWidth();

    // Waveforms take 70%, meter takes 30%
    int waveformWidth = waveWidth * 7 / 10 - static_cast<int>(5 * scaleFactor_);
    inputWaveform_.setBounds(waveBounds.removeFromLeft(waveformWidth));
    outputWaveform_.setBounds(waveBounds.removeFromLeft(waveformWidth));
    gainMeter_.setBounds(waveBounds);

    bounds.removeFromTop(static_cast<int>(10 * scaleFactor_));

    // Type selector section
    int typeHeight = static_cast<int>(35 * scaleFactor_);
    auto typeBounds = bounds.removeFromTop(typeHeight);
    typeBounds.setWidth(static_cast<int>(180 * scaleFactor_));
    typeSelector_->setBounds(typeBounds);

    bounds.removeFromTop(static_cast<int>(10 * scaleFactor_));

    // Knobs section
    int knobHeight = static_cast<int>(100 * scaleFactor_);
    auto knobBounds = bounds.removeFromTop(knobHeight);

    int numKnobs = 6;
    int knobWidth = knobBounds.getWidth() / numKnobs;

    // Scale knob size
    float knobScale = juce::jmin(scaleFactor_, 1.5f);
    int knobSize = static_cast<int>(70 * knobScale);

    // Center knobs in their areas
    int startX = knobBounds.getX();
    for (int i = 0; i < numKnobs; ++i) {
        int areaX = startX + i * knobWidth;
        int areaWidth = knobWidth;
        int knobX = areaX + (areaWidth - knobSize) / 2;
        int knobY = knobBounds.getY() + (knobHeight - knobSize) / 2;

        switch (i) {
            case 0: driveKnob_->setBounds(knobX, knobY, knobSize, knobSize); break;
            case 1: toneKnob_->setBounds(knobX, knobY, knobSize, knobSize); break;
            case 2: outputKnob_->setBounds(knobX, knobY, knobSize, knobSize); break;
            case 3: mixKnob_->setBounds(knobX, knobY, knobSize, knobSize); break;
            case 4: depthKnob_->setBounds(knobX, knobY, knobSize, knobSize); break;
            case 5: attackKnob_->setBounds(knobX, knobY, knobSize, knobSize); break;
        }
    }

    bounds.removeFromTop(static_cast<int>(10 * scaleFactor_));

    // Preset and options section
    int presetHeight = static_cast<int>(35 * scaleFactor_);
    auto presetBounds = bounds.removeFromTop(presetHeight);
    int presetWidth = presetBounds.getWidth();

    // Left side: Presets
    int presetComboWidth = static_cast<int>(150 * scaleFactor_);
    presetCombo_.setBounds(presetBounds.getX(), presetBounds.getY(),
                          presetComboWidth, presetHeight);
    savePresetButton_.setBounds(presetBounds.getX() + presetComboWidth + static_cast<int>(5 * scaleFactor_),
                                presetBounds.getY(), static_cast<int>(50 * scaleFactor_), presetHeight);
    loadPresetButton_.setBounds(presetBounds.getX() + presetComboWidth + static_cast<int>(60 * scaleFactor_),
                                presetBounds.getY(), static_cast<int>(50 * scaleFactor_), presetHeight);
    abCompareButton_.setBounds(presetBounds.getX() + presetComboWidth + static_cast<int>(115 * scaleFactor_),
                              presetBounds.getY(), static_cast<int>(50 * scaleFactor_), presetHeight);

    // Right side: Options
    int optionX = presetBounds.getX() + presetWidth - static_cast<int>(120 * scaleFactor_);
    oversampleToggle_.setBounds(optionX, presetBounds.getY(),
                               static_cast<int>(120 * scaleFactor_), presetHeight);

    bounds.removeFromTop(static_cast<int>(10 * scaleFactor_));

    // Status bar
    statusLabel_.setBounds(bounds);
}

void PluginEditor::updateStatus(const juce::String& message) {
    statusLabel_.setText(message, juce::dontSendNotification);
}

void PluginEditor::paint(juce::Graphics& g) {
    // Background with subtle gradient
    g.fillAll(juce::Colours::darkgrey.darker(0.5f));

    // Border
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1), 4, 1);
}

}  // namespace DistortionPro
