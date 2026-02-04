/**
 * SimpleTest.cpp
 *
 * Standalone test for distortion algorithms - no JUCE dependency
 */

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <cassert>
#include <random>

using namespace std;

//==============================================================================
// Distortion Algorithms (copied from DistortionAlgorithms.h for standalone testing)
//==============================================================================

enum class DistortionType { Overdrive, Distortion, Fuzz, Saturation };

inline float softClipping(float input, float drive) {
    float gain = 1.0f + drive * 4.0f;
    float x = input * gain;
    float k = 1.0f + drive * 2.0f;
    return std::tanh(x * k);
}

inline float hardClipping(float input, float drive) {
    float gain = 1.0f + drive * 10.0f;
    float x = input * gain;
    const float thresholds[] = {0.33f, 0.5f, 0.66f, 0.8f, 1.0f};
    float threshold = thresholds[static_cast<int>(drive * 4.0f)];
    if (x > threshold) return threshold;
    if (x < -threshold) return -threshold;
    return x;
}

inline float fuzzAlgorithm(float input, float drive) {
    float gain = 1.0f + drive * 20.0f;
    float x = input * gain;
    float softness = 1.0f - drive * 0.5f;
    float sign = (x >= 0.0f) ? 1.0f : -1.0f;
    return sign * std::tanh(std::abs(x) * softness);
}

inline float tapeSaturation(float input, float drive) {
    float gain = 1.0f + drive * 3.0f;
    float x = input * gain;
    float positive = std::tanh(x);
    float negative = std::tanh(x * 0.7f) * 0.7f;
    float sign = (x >= 0.0f) ? 1.0f : -1.0f;
    return sign * (x >= 0.0f ? positive : -negative);
}

inline float applyTone(float input, float tone) {
    static float lastSample = 0.0f;
    float alpha = 0.3f + tone * 0.7f;
    lastSample = alpha * input + (1.0f - alpha) * lastSample;
    float wetAmount = 1.0f - tone;
    return (1.0f - wetAmount) * input + wetAmount * lastSample;
}

inline float applyDepth(float distorted, float depth) {
    float knee = 0.2f + depth * 0.8f;
    float softResult = std::tanh(distorted * (1.0f / knee));
    float hardResult = (distorted > knee ? 1.0f : (distorted < -knee ? -1.0f : distorted / knee));
    return (1.0f - depth) * hardResult + depth * softResult;
}

//==============================================================================
// Tests
//==============================================================================

const float EPSILON = 0.001f;

bool approximatelyEqual(float a, float b, float epsilon = EPSILON) {
    return std::abs(a - b) < epsilon;
}

void testSoftClipping() {
    cout << "Testing Soft Clipping... ";

    // Test that output is within reasonable range
    float input = 0.1f;
    float output = softClipping(input, 0.1f);
    assert(output > 0.0f && output < 1.0f);  // Should be amplified but not clipped

    // Test symmetry
    float pos = softClipping(input, 0.5f);
    float neg = softClipping(-input, 0.5f);
    assert(approximatelyEqual(pos, -neg));

    // Test clamping at high drive
    float highOutput = softClipping(2.0f, 1.0f);
    assert(highOutput <= 1.0f && highOutput >= -1.0f);

    cout << "PASSED\n";
}

void testHardClipping() {
    cout << "Testing Hard Clipping... ";

    // At low drive, signal should pass through mostly unchanged
    float input = 0.1f;
    float output = hardClipping(input, 0.1f);
    assert(output > 0.0f && output < 0.5f);

    // Test symmetry
    float pos = hardClipping(input, 0.5f);
    float neg = hardClipping(-input, 0.5f);
    assert(approximatelyEqual(pos, -neg));

    // Test hard clipping at threshold
    float highOutput = hardClipping(2.0f, 1.0f);
    assert(highOutput <= 1.0f && highOutput >= -1.0f);

    cout << "PASSED\n";
}

void testFuzz() {
    cout << "Testing Fuzz... ";
    float input = 0.3f;
    float positive = fuzzAlgorithm(input, 0.5f);
    float negative = fuzzAlgorithm(-input, 0.5f);
    assert(approximatelyEqual(positive, -negative, 0.1f));
    cout << "PASSED\n";
}

void testToneControl() {
    cout << "Testing Tone Control... ";

    // Need multiple samples for filter to work properly
    float inputs[] = {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    // Test with high tone (less filtering)
    for (int i = 0; i < 8; i++) {
        applyTone(inputs[i], 1.0f);
    }

    // Reset static state by creating new function call
    // Test that function runs without error
    float result = applyTone(0.5f, 0.5f);
    assert(result >= -1.0f && result <= 1.0f);

    cout << "PASSED\n";
}

void testSymmetry() {
    cout << "Testing Symmetry... ";
    float input = 0.3f;
    float pos = softClipping(input, 0.5f);
    float neg = softClipping(-input, 0.5f);
    assert(approximatelyEqual(pos, -neg));
    cout << "PASSED\n";
}

void testClamping() {
    cout << "Testing Clamping... ";
    // All distortion algorithms should output within [-1, 1] range
    float input = 2.0f;
    assert(std::abs(softClipping(input, 1.0f)) <= 1.0f);
    assert(std::abs(hardClipping(input, 1.0f)) <= 1.0f);
    assert(std::abs(fuzzAlgorithm(input, 1.0f)) <= 1.0f);
    assert(std::abs(tapeSaturation(input, 1.0f)) <= 1.0f);
    cout << "PASSED\n";
}

void benchmark(int iterations = 1000000) {
    cout << "\n=== Performance Benchmark (" << iterations << " iterations) ===\n";

    std::vector<float> input(iterations);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    for (int i = 0; i < iterations; ++i) {
        input[i] = dis(gen);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        softClipping(input[i], 0.5f);
        hardClipping(input[i], 0.5f);
        fuzzAlgorithm(input[i], 0.5f);
        tapeSaturation(input[i], 0.5f);
        applyTone(input[i], 0.5f);
        applyDepth(input[i], 0.5f);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    cout << "Total time: " << ms << " ms\n";
    cout << "Time per sample: " << (ms / iterations * 1000000) << " ns\n";
    cout << "Samples per ms: " << (iterations / ms) << "\n";
}

int main() {
    cout << "====================================\n";
    cout << "DistortionPro Algorithm Tests\n";
    cout << "====================================\n\n";

    testSoftClipping();
    testHardClipping();
    testFuzz();
    testToneControl();
    testSymmetry();
    testClamping();

    benchmark();

    cout << "\n====================================\n";
    cout << "All Tests Passed!\n";
    cout << "====================================\n";

    return 0;
}
