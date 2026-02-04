/**
 * KnobComponent.h
 *
 * Rotary knob control for plugin parameters
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace DistortionPro {

class KnobComponent : public juce::Component,
                      public juce::Slider::Listener {
public:
    KnobComponent(const juce::String& name, juce::AudioProcessorValueTreeState& valueTree,
                   const juce::String& paramId);
    ~KnobComponent() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void sliderValueChanged(juce::Slider* slider) override;

    void setLabel(const juce::String& newLabel);
    void setValueDisplayFunction(std::function<juce::String(float)> func);

private:
    juce::String label_;
    juce::Slider slider_;
    juce::Label valueLabel_;
    juce::Label nameLabel_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment_;

    std::function<juce::String(float)> valueDisplayFunc_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobComponent)
};

}  // namespace DistortionPro
