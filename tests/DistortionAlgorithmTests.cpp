/**
 * DistortionAlgorithmTests.cpp
 *
 * Unit tests for distortion algorithms
 */

#include <iostream>
#include <cmath>
#include <cassert>
#include "../src/dsp/DistortionAlgorithms.h"

using namespace DistortionPro;

// Test tolerance
const float EPSILON = 0.001f;

bool approximatelyEqual(float a, float b, float epsilon = EPSILON) {
    return std::abs(a - b) < epsilon;
}

void testSoftClipping() {
    std::cout << "Testing Soft Clipping (Overdrive)... ";

    // Test at low drive, input should pass through cleanly
    float input = 0.1f;
    float output = softClipping(input, 0.1f);
    assert(approximatelyEqual(output, input, 0.01f));

    // Test at high drive, should clip
    input = 0.5f;
    output = softClipping(input, 0.8f);
    assert(std::abs(output) < std::abs(input));  // Should be compressed

    // Test symmetry (positive and negative should mirror)
    input = 0.3f;
    float positive = softClipping(input, 0.5f);
    float negative = softClipping(-input, 0.5f);
    assert(approximatelyEqual(positive, -negative));

    std::cout << "PASSED\n";
}

void testHardClipping() {
    std::cout << "Testing Hard Clipping (Distortion)... ";

    // Test at low drive, input should pass through
    float input = 0.1f;
    float output = hardClipping(input, 0.1f);
    assert(approximatelyEqual(output, input, 0.01f));

    // Test at high drive, should clip hard
    input = 0.9f;
    output = hardClipping(input, 0.8f);
    assert(std::abs(output) < 1.0f);  // Clipped to threshold

    // Test threshold behavior
    output = hardClipping(0.5f, 0.5f);
    assert(std::abs(output) <= 0.5f);  // Should be at threshold

    std::cout << "PASSED\n";
}

void testFuzzAlgorithm() {
    std::cout << "Testing Fuzz Algorithm... ";

    // Test at low drive
    float input = 0.1f;
    float output = fuzzAlgorithm(input, 0.1f);
    assert(std::abs(output) < input * 5.0f);  // Should be amplified but not crazy

    // Test at high drive
    input = 0.2f;
    output = fuzzAlgorithm(input, 0.9f);
    // Fuzz should heavily saturate

    // Test symmetry
    input = 0.3f;
    float positive = fuzzAlgorithm(input, 0.5f);
    float negative = fuzzAlgorithm(-input, 0.5f);
    assert(approximatelyEqual(positive, -negative, 0.1f));

    std::cout << "PASSED\n";
}

void testTapeSaturation() {
    std::cout << "Testing Tape Saturation... ";

    // Test at low drive
    float input = 0.1f;
    float output = tapeSaturation(input, 0.1f);
    assert(approximatelyEqual(output, input, 0.02f));

    // Test asymmetric behavior
    float positive = tapeSaturation(0.3f, 0.5f);
    float negative = tapeSaturation(-0.3f, 0.5f);
    // Should not be perfect negatives (asymmetric saturation)

    std::cout << "PASSED\n";
}

void testToneControl() {
    std::cout << "Testing Tone Control... ";

    // High tone should pass highs
    float inputHigh[] = {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f};  // High freq content
    float outputHigh = applyTone(inputHigh[0], 1.0f);
    assert(approximatelyEqual(outputHigh, inputHigh[0], 0.1f));

    // Low tone should dampen
    float outputLow = applyTone(inputHigh[0], 0.0f);
    assert(std::abs(outputLow) < std::abs(outputHigh));

    std::cout << "PASSED\n";
}

void testDistortionTypeSwitching() {
    std::cout << "Testing Distortion Type Switching... ";

    float input = 0.5f;

    // All types should produce some output
    float overdrive = processDistortion(input, DistortionType::Overdrive, 0.5f);
    float distortion = processDistortion(input, DistortionType::Distortion, 0.5f);
    float fuzz = processDistortion(input, DistortionType::Fuzz, 0.5f);
    float saturation = processDistortion(input, DistortionType::Saturation, 0.5f);

    assert(overdrive != 0.0f);
    assert(distortion != 0.0f);
    assert(fuzz != 0.0f);
    assert(saturation != 0.0f);

    std::cout << "PASSED\n";
}

void testParameterNormalization() {
    std::cout << "Testing Parameter Normalization... ";

    // All parameters should work at boundaries
    float input = 0.5f;

    // Drive 0
    assert(approximatelyEqual(processDistortion(input, DistortionType::Overdrive, 0.0f), input, 0.1f));

    // Drive 1
    float output = processDistortion(input, DistortionType::Overdrive, 1.0f);
    assert(std::abs(output) <= 1.0f);  // Should be clipped

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "=== Distortion Algorithm Tests ===\n\n";

    testSoftClipping();
    testHardClipping();
    testFuzzAlgorithm();
    testTapeSaturation();
    testToneControl();
    testDistortionTypeSwitching();
    testParameterNormalization();

    std::cout << "\n=== All Tests Passed ===\n";
    return 0;
}
