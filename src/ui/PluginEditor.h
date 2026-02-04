/**
 * PluginEditor.h
 *
 * Main plugin editor UI component with high-DPI and responsive layout support
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../plugin/DistortionPro.h"
#include "KnobComponent.h"
#include "WaveformDisplay.h"
#include "TypeSelector.h"
#include "GainMeter.h"

namespace DistortionPro {

class PluginEditor : public juce::AudioProcessorEditor {
public:
    PluginEditor(DistortionPro& processor);
    ~PluginEditor() override;

    // High-DPI support
    void paint(juce::Graphics& g) override;
    void resized() override;

    // Component callbacks
    void childBoundsChanged(juce::Component* child) override;

private:
    DistortionPro& processor_;
    juce::AudioProcessorValueTreeState& valueTree_;

    // Scale factor for high-DPI displays
    float scaleFactor_ = 1.0f;

    // Minimum dimensions
    static constexpr int minWidth = 600;
    static constexpr int minHeight = 400;

    // Main layout components
    juce::Label titleLabel_;
    juce::Label subtitleLabel_;

    // Waveform displays
    WaveformDisplay inputWaveform_;
    WaveformDisplay outputWaveform_;

    // Gain meter
    GainMeter gainMeter_;

    // Type selector
    TypeSelector* typeSelector_;

    // Knobs
    KnobComponent* driveKnob_;
    KnobComponent* toneKnob_;
    KnobComponent* outputKnob_;
    KnobComponent* mixKnob_;
    KnobComponent* depthKnob_;
    KnobComponent* attackKnob_;

    // Preset controls
    juce::ComboBox presetCombo_;
    juce::TextButton savePresetButton_;
    juce::TextButton loadPresetButton_;
    juce::TextButton abCompareButton_;

    // Options
    juce::ToggleButton oversampleToggle_;

    // Status bar
    juce::Label statusLabel_;

    void setupUI();
    void setupKnobs();
    void setupPresets();
    void setupOptions();
    void setupStatusBar();
    void layoutComponents();

    void updateScaleFactor();
    void updateStatus(const juce::String& message);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};

}  // namespace DistortionPro
