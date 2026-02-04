/**
 * ABCompareComponent.h
 *
 * A/B comparison component for comparing two sets of parameters
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../dsp/DistortionProcessor.h"

namespace DistortionPro {

class ABCompareComponent : public juce::Component {
public:
    ABCompareComponent(juce::AudioProcessorValueTreeState& valueTree);
    ~ABCompareComponent() override;

    void setEnabled(bool enabled);
    bool isEnabled() const { return enabled_; }

    void saveCurrentAsA();
    void saveCurrentAsB();
    void compare();

    void paint(juce::Graphics& g) override;
    void resized() override;

    std::function<void()> onCompareChanged;

private:
    bool enabled_ = false;
    ProcessorParams paramsA_;
    ProcessorParams paramsB_;

    juce::AudioProcessorValueTreeState& valueTree_;

    juce::ToggleButton compareToggle_;
    juce::Label labelA_;
    juce::Label labelB_;

    void setupUI();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ABCompareComponent)
};

}  // namespace DistortionPro
