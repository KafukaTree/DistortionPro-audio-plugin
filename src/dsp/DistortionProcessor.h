/**
 * DistortionProcessor.h
 *
 * Main audio processor for distortion effect
 * Handles all parameters, algorithms, and oversampling
 */

#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "DistortionAlgorithms.h"
#include "Oversampler.h"
#include <atomic>

namespace DistortionPro {

// Parameter IDs for automation
enum class ParameterID {
    Drive = 0,
    Tone,
    Output,
    Mix,
    Depth,
    Attack,
    Oversample
};

/**
 * Processor parameters structure
 */
struct ProcessorParams {
    // Normalized parameters (0.0 to 1.0)
    float drive = 0.5f;
    float tone = 0.5f;
    float output = 0.75f;
    float mix = 1.0f;
    float depth = 0.5f;
    float attack = 0.5f;

    // Discrete parameter
    DistortionType type = DistortionType::Overdrive;

    // Boolean parameters
    bool oversample = false;
};

/**
 * Main distortion processor class
 */
class DistortionProcessor {
public:
    DistortionProcessor();
    ~DistortionProcessor();

    /**
     * Initialize the processor
     */
    void initialize(double sampleRate, int maxSamplesPerBlock);

    /**
     * Reset processor state
     */
    void reset();

    /**
     * Process audio block
     */
    void process(juce::AudioBuffer<float>& buffer);

    /**
     * Set a parameter value
     */
    void setParameter(ParameterID id, float value);

    /**
     * Get a parameter value
     */
    float getParameter(ParameterID id) const;

    /**
     * Set distortion type
     */
    void setDistortionType(DistortionType type);

    /**
     * Get current distortion type
     */
    DistortionType getDistortionType() const;

    /**
     * Get total latency in samples
     */
    int getLatency() const { return latency_; }

    /**
     * Set oversampling enabled state
     */
    void setOversampling(bool enabled);

    /**
     * Check if oversampling is enabled
     */
    bool isOversampling() const { return oversamplingEnabled_; }

    /**
     * Get current parameters
     */
    const ProcessorParams& getParams() const { return params_; }

private:
    // Sample rate
    double sampleRate_ = 44100.0;

    // Parameters
    ProcessorParams params_;

    // Oversampling
    Oversampler oversampler_;
    bool oversamplingEnabled_ = false;
    int latency_ = 0;

    // Attack envelope follower
    float attackEnvelope_ = 0.0f;

    // Wet/dry mixing buffers
    juce::AudioBuffer<float> wetBuffer_;
    juce::AudioBuffer<float> dryBuffer_;

    // Process with oversampling
    void processWithOversampling(juce::AudioBuffer<float>& buffer);

    // Process without oversampling
    void processWithoutOversampling(juce::AudioBuffer<float>& buffer);

    // Apply attack parameter
    float applyAttack(float input);

    // Clamp helper
    template<typename T>
    static T clamp(T value, T minVal, T maxVal) {
        return value < minVal ? minVal : (value > maxVal ? maxVal : value);
    }
};

}  // namespace DistortionPro
