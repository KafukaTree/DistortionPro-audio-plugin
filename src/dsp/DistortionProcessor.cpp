/**
 * DistortionProcessor.cpp
 *
 * Implementation of the distortion audio processor
 */

#include "DistortionProcessor.h"

namespace DistortionPro {

DistortionProcessor::DistortionProcessor() {
}

DistortionProcessor::~DistortionProcessor() {
}

void DistortionProcessor::initialize(double sampleRate, int maxSamplesPerBlock) {
    sampleRate_ = sampleRate;

    // Initialize oversampler
    oversampler_.initialize(sampleRate, 2);
    latency_ = oversampler_.getLatency();

    // Allocate processing buffers
    wetBuffer_.setSize(2, maxSamplesPerBlock * 2);
    dryBuffer_.setSize(2, maxSamplesPerBlock);

    reset();
}

void DistortionProcessor::reset() {
    oversampler_.reset();
    attackEnvelope_ = 0.0f;
    wetBuffer_.clear();
    dryBuffer_.clear();
}

void DistortionProcessor::process(juce::AudioBuffer<float>& buffer) {
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    (void)numSamples;
    (void)numChannels;

    if (oversamplingEnabled_) {
        processWithOversampling(buffer);
    } else {
        processWithoutOversampling(buffer);
    }
}

void DistortionProcessor::processWithOversampling(juce::AudioBuffer<float>& buffer) {
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    // Store dry signal for mixing
    dryBuffer_.makeCopyOf(buffer);

    int oversampledNum = numSamples * 2;
    wetBuffer_.setSize(numChannels, oversampledNum);
    wetBuffer_.clear();

    // Upsample each channel
    std::vector<float> upBuffer(oversampledNum);
    for (int ch = 0; ch < numChannels; ++ch) {
        oversampler_.upsample(buffer.getReadPointer(ch), upBuffer.data(), numSamples);

        // Process at oversampled rate
        for (int i = 0; i < oversampledNum; ++i) {
            float distorted = processDistortion(upBuffer[i], params_.type, params_.drive);
            distorted = applyDepth(distorted, params_.depth);
            distorted = applyTone(distorted, params_.tone);
            upBuffer[i] = distorted;
        }

        // Downsample back
        oversampler_.downsample(upBuffer.data(), wetBuffer_.getWritePointer(ch), numSamples);
    }

    // Mix dry and wet
    for (int ch = 0; ch < numChannels; ++ch) {
        float* dry = dryBuffer_.getWritePointer(ch);
        float* wet = wetBuffer_.getWritePointer(ch);
        float* out = buffer.getWritePointer(ch);

        for (int i = 0; i < numSamples; ++i) {
            float wetGain = wet[i] * params_.output;
            float dryGain = dry[i] * (1.0f - params_.mix * 0.5f);
            out[i] = dryGain * (1.0f - params_.mix) + wetGain * params_.mix;
        }
    }
}

void DistortionProcessor::processWithoutOversampling(juce::AudioBuffer<float>& buffer) {
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    // Store dry signal for mixing
    dryBuffer_.makeCopyOf(buffer);

    for (int ch = 0; ch < numChannels; ++ch) {
        float* input = buffer.getWritePointer(ch);

        for (int i = 0; i < numSamples; ++i) {
            float distorted = processDistortion(input[i], params_.type, params_.drive);
            distorted = applyDepth(distorted, params_.depth);
            distorted = applyTone(distorted, params_.tone);

            float wetGain = distorted * params_.output;
            float dryGain = input[i] * (1.0f - params_.mix * 0.5f);
            input[i] = dryGain * (1.0f - params_.mix) + wetGain * params_.mix;
        }
    }
}

float DistortionProcessor::applyAttack(float input) {
    float attackRate = 0.1f + params_.attack * 0.9f;
    attackEnvelope_ = attackEnvelope_ * (1.0f - attackRate) + std::abs(input) * attackRate;
    return attackEnvelope_;
}

void DistortionProcessor::setParameter(ParameterID id, float value) {
    switch (id) {
        case ParameterID::Drive:
            params_.drive = clamp(value, 0.0f, 1.0f);
            break;
        case ParameterID::Tone:
            params_.tone = clamp(value, 0.0f, 1.0f);
            break;
        case ParameterID::Output:
            params_.output = clamp(value, 0.0f, 1.0f);
            break;
        case ParameterID::Mix:
            params_.mix = clamp(value, 0.0f, 1.0f);
            break;
        case ParameterID::Depth:
            params_.depth = clamp(value, 0.0f, 1.0f);
            break;
        case ParameterID::Attack:
            params_.attack = clamp(value, 0.0f, 1.0f);
            break;
        case ParameterID::Oversample:
            oversamplingEnabled_ = value >= 0.5f;
            break;
    }
}

float DistortionProcessor::getParameter(ParameterID id) const {
    switch (id) {
        case ParameterID::Drive:    return params_.drive;
        case ParameterID::Tone:     return params_.tone;
        case ParameterID::Output:   return params_.output;
        case ParameterID::Mix:      return params_.mix;
        case ParameterID::Depth:    return params_.depth;
        case ParameterID::Attack:   return params_.attack;
        case ParameterID::Oversample: return oversamplingEnabled_ ? 1.0f : 0.0f;
        default:                     return 0.0f;
    }
}

void DistortionProcessor::setDistortionType(DistortionType type) {
    params_.type = type;
}

DistortionType DistortionProcessor::getDistortionType() const {
    return params_.type;
}

void DistortionProcessor::setOversampling(bool enabled) {
    oversamplingEnabled_ = enabled;
}

}  // namespace DistortionPro
