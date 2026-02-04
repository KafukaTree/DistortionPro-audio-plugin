/**
 * GainMeter.h
 *
 * Gain reduction meter display component
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <atomic>

namespace DistortionPro {

class GainMeter : public juce::Component,
                  private juce::Timer {
public:
    GainMeter();
    ~GainMeter() override;

    void setGainReduction(float reductionDB);
    void setPeakLevel(float levelDB);
    void setEnabled(bool enabled);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;

    std::atomic<float> gainReduction_{-60.0f};
    std::atomic<float> peakLevel_{-60.0f};
    bool enabled_ = true;

    float smoothedReduction_ = -60.0f;
    float smoothedPeak_ = -60.0f;

    static constexpr float maxReductionDB_ = 0.0f;
    static constexpr float minReductionDB_ = -30.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainMeter)
};

}  // namespace DistortionPro
