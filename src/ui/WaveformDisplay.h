/**
 * WaveformDisplay.h
 *
 * Real-time waveform visualization component
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <atomic>
#include <vector>

namespace DistortionPro {

class WaveformDisplay : public juce::Component,
                        private juce::Timer {
public:
    enum class Mode {
        Input,
        Output
    };

    WaveformDisplay();
    ~WaveformDisplay() override;

    void setMode(Mode newMode);
    void setBufferSize(int numSamples);

    void pushSample(float sample);
    void pushSamples(const float* samples, int numSamples);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setColor(juce::Colour newColor);
    void setBackgroundColor(juce::Colour newColor);

private:
    void timerCallback() override;

    Mode mode_ = Mode::Input;
    int bufferSize_ = 512;
    std::vector<float> samples_;
    int writeIndex_ = 0;

    juce::Colour waveformColor_ = juce::Colours::orange;
    juce::Colour backgroundColor_ = juce::Colours::black.withAlpha(0.5f);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

}  // namespace DistortionPro
