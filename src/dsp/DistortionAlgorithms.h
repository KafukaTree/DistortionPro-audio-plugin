/**
 * DistortionAlgorithms.h
 *
 * Contains implementations of various distortion algorithms:
 * - Overdrive: tanh-based soft clipping
 * - Distortion: 5-stage hard clipping
 * - Fuzz: Square wave synthesis
 * - Saturation: Tape emulation
 */

#pragma once

#include <cmath>
#include <algorithm>

namespace DistortionPro {

// Distortion type enumeration
enum class DistortionType {
    Overdrive,    // tanh soft clipping
    Distortion,   // hard clipping
    Fuzz,         // square wave
    Saturation    // tape emulation
};

/**
 * Soft clipping using tanh function (Overdrive)
 * Provides smooth, musical saturation with even harmonics
 */
inline float softClipping(float input, float drive) {
    // Apply input gain based on drive parameter
    float gain = 1.0f + drive * 4.0f;  // 1x to 5x gain
    float x = input * gain;

    // Tanh soft clipping with knee control
    float k = 1.0f + drive * 2.0f;  // Knee steepness
    return std::tanh(x * k);
}

/**
 * 5-stage hard clipping (Distortion)
 * Aggressive clipping with odd harmonics emphasis
 */
inline float hardClipping(float input, float drive) {
    float gain = 1.0f + drive * 10.0f;  // Higher gain for distortion
    float x = input * gain;

    // 5-stage hard clipping thresholds
    const float thresholds[] = {0.33f, 0.5f, 0.66f, 0.8f, 1.0f};
    float threshold = thresholds[static_cast<int>(drive * 4.0f)];

    if (x > threshold) return threshold;
    if (x < -threshold) return -threshold;
    return x;
}

/**
 * Square wave-based fuzz algorithm
 * Creates thick, fuzzy texture with extreme saturation
 */
inline float fuzzAlgorithm(float input, float drive) {
    float gain = 1.0f + drive * 20.0f;  // Very high gain
    float x = input * gain;

    // Square wave with soft edges
    float softness = 1.0f - drive * 0.5f;

    // Asymptotic approach to square wave
    if (x > 0.0f) {
        return std::tanh(x * softness) * std::copysign(1.0f, x);
    } else {
        return std::tanh(x * softness);
    }
}

/**
 * Tape saturation emulation
 * Simulates magnetic tape compression and harmonic enhancement
 */
inline float tapeSaturation(float input, float drive) {
    // Input gain with auto-makeup
    float gain = 1.0f + drive * 3.0f;
    float x = input * gain;

    // Asymmetric saturation curve (tape characteristic)
    float positive = std::tanh(x);
    float negative = std::tanh(x * 0.7f) * 0.7f;  // Asymmetric

    // Blend based on input polarity
    float sign = (x >= 0.0f) ? 1.0f : -1.0f;
    return sign * (x >= 0.0f ? positive : -negative);
}

/**
 * Main distortion processor function
 * Selects and applies the appropriate distortion algorithm
 */
inline float processDistortion(float input, DistortionType type, float drive) {
    switch (type) {
        case DistortionType::Overdrive:
            return softClipping(input, drive);

        case DistortionType::Distortion:
            return hardClipping(input, drive);

        case DistortionType::Fuzz:
            return fuzzAlgorithm(input, drive);

        case DistortionType::Saturation:
            return tapeSaturation(input, drive);

        default:
            return input;
    }
}

/**
 * Apply tone control (high frequency dampening)
 */
inline float applyTone(float input, float tone) {
    // Simple first-order low-pass filter
    // tone=0: full dampening, tone=1: no dampening
    float alpha = 0.3f + tone * 0.7f;

    // Simplified tone shaping
    static float lastSample = 0.0f;
    lastSample = alpha * input + (1.0f - alpha) * lastSample;

    // Blend between original and filtered
    float wetAmount = 1.0f - tone;
    return (1.0f - wetAmount) * input + wetAmount * lastSample;
}

/**
 * Apply depth parameter (softness/knee control)
 */
inline float applyDepth(float distorted, float depth) {
    // Depth affects the transition characteristic
    // Higher depth = softer knee
    float knee = 0.2f + depth * 0.8f;

    // Blend between hard and soft clipping characteristic
    float softResult = std::tanh(distorted * (1.0f / knee));
    float hardResult = (distorted > knee ? 1.0f : (distorted < -knee ? -1.0f : distorted / knee));

    return (1.0f - depth) * hardResult + depth * softResult;
}

}  // namespace DistortionPro
