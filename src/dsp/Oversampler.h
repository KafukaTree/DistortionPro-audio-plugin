/**
 * Oversampler.h
 *
 * 2x oversampler for reduced aliasing in distortion processing
 */

#pragma once

#include <vector>
#include <cmath>

namespace DistortionPro {

class Oversampler {
public:
    Oversampler();
    ~Oversampler();

    /**
     * Initialize the oversampler
     * @param baseSampleRate Original sample rate
     * @param factor Oversampling factor (2 for 2x)
     */
    void initialize(double baseSampleRate, int factor = 2);

    /**
     * Process input samples at higher sample rate
     * @param input Input buffer at base sample rate
     * @param output Output buffer at oversampled rate
     * @param numSamples Number of samples at base rate
     */
    void upsample(const float* input, float* output, int numSamples);

    /**
     * Downsample back to base sample rate
     * @param input Input buffer at oversampled rate
     * @param output Output buffer at base sample rate
     * @param numSamples Number of samples at base rate
     */
    void downsample(const float* input, float* output, int numSamples);

    /**
     * Get the oversampling factor
     */
    int getFactor() const { return factor_; }

    /**
     * Get the current sample rate
     */
    double getSampleRate() const { return currentSampleRate_; }

    /**
     * Get the latency in samples at base rate
     */
    int getLatency() const { return latency_; }

    /**
     * Reset internal state
     */
    void reset();

private:
    int factor_ = 2;
    double currentSampleRate_ = 44100.0;
    int latency_ = 0;

    // Fir filter coefficients for 2x upsampling
    std::vector<float> upsampleCoeffs_;
    std::vector<float> downsampleCoeffs_;

    // Internal state buffers
    std::vector<float> upsampleState_;
    std::vector<float> downsampleState_;

    // Design interpolation filter
    void designFirFilters();
};

}  // namespace DistortionPro
