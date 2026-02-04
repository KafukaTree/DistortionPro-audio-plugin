/**
 * Oversampler.cpp
 *
 * 2x oversampler implementation using FIR filters
 */

#include "Oversampler.h"
#include <cstring>

namespace DistortionPro {

Oversampler::Oversampler() {
}

Oversampler::~Oversampler() {
}

void Oversampler::initialize(double baseSampleRate, int factor) {
    factor_ = factor;
    currentSampleRate_ = baseSampleRate;

    // Design FIR filters for 2x oversampling
    designFirFilters();

    // Calculate latency (half of filter length)
    latency_ = static_cast<int>(upsampleCoeffs_.size()) / 2;

    // Initialize state buffers
    upsampleState_.assign(upsampleCoeffs_.size(), 0.0f);
    downsampleState_.assign(downsampleCoeffs_.size(), 0.0f);
}

void Oversampler::designFirFilters() {
    // Simple FIR filter design for 2x interpolation
    // 16-tap linear phase FIR filter

    // Coefficients for interpolation filter (sinc-based)
    // Normalized for 2x upsampling
    upsampleCoeffs_ = {
        -0.0013f, -0.0021f,  0.0048f,  0.0117f,
        -0.0221f, -0.0391f,  0.0664f,  0.1298f,
         0.1298f,  0.0664f, -0.0391f, -0.0221f,
         0.0117f,  0.0048f, -0.0021f, -0.0013f
    };

    // Decimation filter coefficients (same as interpolation, reversed)
    downsampleCoeffs_ = upsampleCoeffs_;
}

void Oversampler::upsample(const float* input, float* output, int numSamples) {
    const int filterLen = static_cast<int>(upsampleCoeffs_.size());
    const int upNumSamples = numSamples * factor_;

    // Zero-pad and interpolate
    for (int i = 0; i < upNumSamples; ++i) {
        if (i % factor_ == 0) {
            // Insert new sample
            output[i] = input[i / factor_];
        } else {
            // Interpolate using FIR filter
            float sum = 0.0f;
            int inputIdx = i / factor_;

            for (int j = 0; j < filterLen; ++j) {
                int sampleIdx = inputIdx - filterLen / 2 + j;

                if (sampleIdx >= 0 && sampleIdx < numSamples) {
                    sum += input[sampleIdx] * upsampleCoeffs_[j];
                }
            }

            // Add interpolated sample
            output[i] = sum * factor_;
        }
    }
}

void Oversampler::downsample(const float* input, float* output, int numSamples) {
    const int filterLen = static_cast<int>(downsampleCoeffs_.size());
    const int upNumSamples = numSamples * factor_;

    // Apply decimation filter and downsample
    for (int i = 0; i < numSamples; ++i) {
        float sum = 0.0f;

        for (int j = 0; j < filterLen; ++j) {
            int sampleIdx = i * factor_ + j;

            if (sampleIdx < upNumSamples) {
                sum += input[sampleIdx] * downsampleCoeffs_[j];
            }
        }

        output[i] = sum;
    }
}

void Oversampler::reset() {
    std::fill(upsampleState_.begin(), upsampleState_.end(), 0.0f);
    std::fill(downsampleState_.begin(), downsampleState_.end(), 0.0f);
}

}  // namespace DistortionPro
