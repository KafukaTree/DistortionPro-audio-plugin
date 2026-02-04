/**
 * TypeSelector.h
 *
 * Distortion type selector dropdown component
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../dsp/DistortionAlgorithms.h"

namespace DistortionPro {

class TypeSelector : public juce::Component,
                     public juce::ComboBox::Listener {
public:
    TypeSelector(juce::AudioProcessorValueTreeState& valueTree);
    ~TypeSelector() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void comboBoxChanged(juce::ComboBox* comboBox) override;

    DistortionType getSelectedType() const;
    void setSelectedType(DistortionType type);

private:
    juce::ComboBox comboBox_;
    juce::Label label_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment_;

    void populateComboBox();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TypeSelector)
};

}  // namespace DistortionPro
