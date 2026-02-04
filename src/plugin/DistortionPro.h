/**
 * DistortionPro.h
 *
 * Main JUCE plugin header file - JUCE 7 compatible
 */

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../dsp/DistortionProcessor.h"

namespace DistortionPro {

/**
 * Main audio processor class for DistortionPro plugin
 */
class DistortionPro : public juce::AudioProcessor {
public:
    DistortionPro();
    ~DistortionPro() override;

    // AudioProcessor overrides
    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    // Editor
    bool hasEditor() const override;
    juce::AudioProcessorEditor* createEditor() override;

    // State
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    // Programs
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    // State serialization
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Custom methods
    DistortionProcessor& getProcessor() { return processor_; }

    // Parameter access
    juce::AudioProcessorValueTreeState& getValueTreeState() { return *valueTreeState_; }

    // Parameter layout
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    DistortionProcessor processor_;
    std::unique_ptr<juce::AudioProcessorValueTreeState> valueTreeState_;

    struct ProgramData {
        juce::String name;
        ProcessorParams params;
    };

    std::vector<ProgramData> programs_;
    int currentProgram_ = 0;

    void initializePrograms();
    void createParameters();
    void applyProgram(int programIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionPro)
};

}  // namespace DistortionPro
