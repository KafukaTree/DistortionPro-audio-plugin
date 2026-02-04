/**
 * WaveformDisplay.cpp
 *
 * Real-time waveform visualization implementation
 */

#include "WaveformDisplay.h"

namespace DistortionPro {

WaveformDisplay::WaveformDisplay() {
    samples_.assign(bufferSize_, 0.0f);

    // Start update timer (60 FPS)
    startTimer(1000 / 60);
}

WaveformDisplay::~WaveformDisplay() {
}

void WaveformDisplay::setMode(Mode newMode) {
    mode_ = newMode;
}

void WaveformDisplay::setBufferSize(int numSamples) {
    bufferSize_ = numSamples;
    samples_.assign(bufferSize_, 0.0f);
    writeIndex_ = 0;
}

void WaveformDisplay::pushSample(float sample) {
    samples_[writeIndex_] = sample;
    writeIndex_ = (writeIndex_ + 1) % bufferSize_;
}

void WaveformDisplay::pushSamples(const float* samples, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        pushSample(samples[i]);
    }
}

void WaveformDisplay::timerCallback() {
    // Trigger repaint on timer
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour(backgroundColor_);
    g.fillRoundedRectangle(bounds, 4.0f);

    // Grid lines
    g.setColour(juce::Colours::grey.withAlpha(0.2f));
    g.drawHorizontalLine(bounds.getCentreY(), bounds.getX(), bounds.getRight());
    g.drawVerticalLine(bounds.getCentreX(), bounds.getY(), bounds.getBottom());

    // Zero line
    g.setColour(juce::Colours::grey.withAlpha(0.5f));
    g.drawHorizontalLine(bounds.getCentreY(), bounds.getX(), bounds.getRight());

    // Draw waveform
    g.setColour(waveformColor_);

    float width = bounds.getWidth();
    float height = bounds.getHeight();
    float centerY = bounds.getCentreY();

    // Calculate points for waveform
    juce::Path path;
    path.startNewSubPath(bounds.getX(), centerY);

    int numVisible = std::min(bufferSize_, static_cast<int>(width));

    for (int x = 0; x < numVisible; ++x) {
        // Read samples in circular buffer order
        int sampleIdx = (writeIndex_ - numVisible + x + bufferSize_) % bufferSize_;
        float sample = samples_[sampleIdx];

        // Scale sample to display
        float y = centerY - sample * height * 0.45f;

        if (x == 0) {
            path.startNewSubPath(x, y);
        } else {
            path.lineTo(x, y);
        }
    }

    g.strokePath(path, juce::PathStrokeType(1.5f));
}

void WaveformDisplay::resized() {
    // Nothing special needed
}

void WaveformDisplay::setColor(juce::Colour newColor) {
    waveformColor_ = newColor;
}

void WaveformDisplay::setBackgroundColor(juce::Colour newColor) {
    backgroundColor_ = newColor;
}

}  // namespace DistortionPro
