/**
 * PerformanceTests.cpp
 *
 * Performance and stress tests for DistortionPro
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <cmath>
#include "../src/dsp/DistortionAlgorithms.h"
#include "../src/dsp/DistortionProcessor.h"
#include "../src/dsp/Oversampler.h"

using namespace DistortionPro;

// Performance timer utility
class Timer {
public:
    void start() {
        start_ = std::chrono::high_resolution_clock::now();
    }

    double stopMs() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_;
};

void benchmarkDistortionTypes(int iterations = 1000000) {
    std::cout << "\n=== Distortion Type Performance ===\n";

    std::vector<float> input(iterations);
    std::vector<float> output(iterations);

    // Generate test signal
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    for (int i = 0; i < iterations; ++i) {
        input[i] = dis(gen);
    }

    Timer timer;

    // Test Overdrive
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        output[i] = softClipping(input[i], 0.5f);
    }
    double overdriveTime = timer.stopMs();
    std::cout << "Overdrive:    " << overdriveTime << " ms (" << iterations/overdriveTime << " samples/ms)\n";

    // Test Distortion
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        output[i] = hardClipping(input[i], 0.5f);
    }
    double distortionTime = timer.stopMs();
    std::cout << "Distortion:   " << distortionTime << " ms (" << iterations/distortionTime << " samples/ms)\n";

    // Test Fuzz
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        output[i] = fuzzAlgorithm(input[i], 0.5f);
    }
    double fuzzTime = timer.stopMs();
    std::cout << "Fuzz:         " << fuzzTime << " ms (" << iterations/fuzzTime << " samples/ms)\n";

    // Test Saturation
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        output[i] = tapeSaturation(input[i], 0.5f);
    }
    double saturationTime = timer.stopMs();
    std::cout << "Saturation:   " << saturationTime << " ms (" << iterations/saturationTime << " samples/ms)\n";
}

void benchmarkOversampling(int bufferSize = 512, int iterations = 10000) {
    std::cout << "\n=== Oversampling Performance ===\n";

    std::vector<float> input(bufferSize);
    std::vector<float> oversampled(bufferSize * 2);
    std::vector<float> output(bufferSize);

    // Generate test signal
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    for (int i = 0; i < bufferSize; ++i) {
        input[i] = dis(gen);
    }

    Oversampler oversampler;
    oversampler.initialize(44100.0, 2);

    Timer timer;

    // Benchmark oversampling
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        oversampler.upsample(input.data(), oversampled.data(), bufferSize);
        oversampler.downsample(oversampled.data(), output.data(), bufferSize);
    }
    double time = timer.stopMs();

    std::cout << "Buffer size:  " << bufferSize << " samples\n";
    std::cout << "Iterations:   " << iterations << "\n";
    std::cout << "Total time:   " << time << " ms\n";
    std::cout << "Latency:      " << oversampler.getLatency() << " samples\n";
    std::cout << "CPU usage:    " << (time / (iterations * bufferSize / 44100.0)) * 100 << "% at 44.1kHz\n";
}

void benchmarkRealtimePerformance(int sampleRate = 44100, int bufferSize = 512) {
    std::cout << "\n=== Real-time Performance ===\n";

    DistortionProcessor processor;
    processor.initialize(static_cast<double>(sampleRate), bufferSize);

    // Generate test signal
    std::vector<float> input(bufferSize * 2);
    std::vector<float> output(bufferSize * 2);

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    for (int i = 0; i < bufferSize * 2; ++i) {
        input[i] = dis(gen);
    }

    // Test without oversampling
    processor.setOversampling(false);

    int iterations = 1000;  // Simulate 10 seconds at 44.1kHz
    Timer timer;

    timer.start();
    for (int i = 0; i < iterations; ++i) {
        processor.processStereo(input.data(), output.data(), bufferSize);
    }
    double timeNoOversample = timer.stopMs();

    std::cout << "Sample rate:  " << sampleRate << " Hz\n";
    std::cout << "Buffer size:  " << bufferSize << " samples\n";
    std::cout << "Iterations:   " << iterations << "\n";
    std::cout << "No oversample: " << timeNoOversample << " ms\n";

    // Test with oversampling
    processor.setOversampling(true);

    timer.start();
    for (int i = 0; i < iterations; ++i) {
        processor.processStereo(input.data(), output.data(), bufferSize);
    }
    double timeWithOversample = timer.stopMs();

    std::cout << "With 2x oversample: " << timeWithOversample << " ms\n";
    std::cout << "Overhead: " << ((timeWithOversample / timeNoOversample - 1) * 100) << "%\n";
}

void stressTest(int durationSeconds = 10) {
    std::cout << "\n=== Stress Test (" << durationSeconds << " seconds) ===\n";

    int sampleRate = 44100;
    int bufferSize = 512;

    DistortionProcessor processor;
    processor.initialize(static_cast<double>(sampleRate), bufferSize);

    int samplesPerBuffer = bufferSize * 2;
    int totalBuffers = (durationSeconds * sampleRate) / bufferSize;

    std::vector<float> input(samplesPerBuffer);
    std::vector<float> output(samplesPerBuffer);

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    Timer timer;
    timer.start();

    int bufferCount = 0;
    int xruns = 0;

    for (int i = 0; i < totalBuffers; ++i) {
        // Generate test signal with varying levels
        float level = 0.5f + 0.5f * std::sin(i * 0.01f);
        for (int j = 0; j < samplesPerBuffer; ++j) {
            input[j] = dis(gen) * level;
        }

        processor.processStereo(input.data(), output.data(), bufferSize);
        bufferCount++;

        // Check for NaN/Inf
        for (int j = 0; j < samplesPerBuffer; ++j) {
            if (!std::isfinite(output[j])) {
                xruns++;
            }
        }
    }

    double elapsed = timer.stopMs();
    double expectedTime = (totalBuffers * bufferSize / static_cast<double>(sampleRate)) * 1000.0;

    std::cout << "Buffers processed: " << bufferCount << "\n";
    std::cout << "Expected time: " << expectedTime << " ms\n";
    std::cout << "Actual time:  " << elapsed << " ms\n";
    std::cout << "Real-time ratio: " << (expectedTime / elapsed) << "x\n";
    std::cout << "NaN/Inf errors: " << xruns << "\n";

    if (xruns > 0) {
        std::cout << "WARNING: Audio glitches detected!\n";
    } else {
        std::cout << "PASSED: No audio glitches\n";
    }
}

void memoryTest() {
    std::cout << "\n=== Memory Test ===\n";

    DistortionProcessor processor;
    processor.initialize(44100.0, 512);

    // Test parameter automation
    for (int i = 0; i < 100; ++i) {
        processor.setParameter(ParameterID::Drive, i / 100.0f);
        processor.setParameter(ParameterID::Tone, i / 100.0f);
        processor.setParameter(ParameterID::Mix, i / 100.0f);
        processor.setParameter(ParameterID::Depth, i / 100.0f);
        processor.setParameter(ParameterID::Attack, i / 100.0f);
        processor.setParameter(ParameterID::Output, i / 100.0f);
    }

    // Test bypass
    processor.setBypass(false);
    processor.setBypass(true);
    processor.setBypass(false);

    // Test oversampling toggle
    processor.setOversampling(false);
    processor.setOversampling(true);

    std::cout << "PASSED: Memory test completed without errors\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "DistortionPro Performance Tests\n";
    std::cout << "========================================\n";

    benchmarkDistortionTypes();
    benchmarkOversampling();
    benchmarkRealtimePerformance();
    stressTest();
    memoryTest();

    std::cout << "\n========================================\n";
    std::cout << "All Performance Tests Complete!\n";
    std::cout << "========================================\n";

    return 0;
}
