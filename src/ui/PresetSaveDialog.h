/**
 * PresetSaveDialog.h
 *
 * Dialog for saving user presets
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace DistortionPro {

class PresetSaveDialog : public juce::DialogWindow {
public:
    PresetSaveDialog(juce::AudioProcessorValueTreeState& valueTree);
    ~PresetSaveDialog() override;

    void closeButtonPressed() override;

    // Callback for when preset is saved
    std::function<void(const juce::String& name)> onPresetSaved;

private:
    juce::AudioProcessorValueTreeState& valueTree_;
    juce::TextEditor nameEditor_;
    juce::TextButton saveButton_;
    juce::TextButton cancelButton_;
    juce::Label nameLabel_;

    void setupUI();
    void savePreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetSaveDialog)
};

}  // namespace DistortionPro
