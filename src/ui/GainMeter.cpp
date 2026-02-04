/**
 * GainMeter.cpp
 *
 * Gain reduction meter implementation
 */

#include "GainMeter.h"

namespace DistortionPro {

GainMeter::GainMeter() {
    startTimer(50);  // 20 FPS update
}

GainMeter::~GainMeter() {
}

void GainMeter::setGainReduction(float reductionDB) {
    gainReduction_.store(reductionDB);
}

void GainMeter::setPeakLevel(float levelDB) {
    peakLevel_.store(levelDB);
}

void GainMeter::setEnabled(bool enabled) {
    enabled_ = enabled;
}

void GainMeter::timerCallback() {
    // Smooth the meter movements
    float targetReduction = gainReduction_.load();
    float targetPeak = peakLevel_.load();

    smoothedReduction_ += (targetReduction - smoothedReduction_) * 0.3f;
    smoothedPeak_ += (targetPeak - smoothedPeak_) * 0.2f;

    repaint();
}

void GainMeter::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.fillRoundedRectangle(bounds, 2.0f);

    if (!enabled_) {
        return;
    }

    float width = bounds.getWidth();
    float height = bounds.getHeight();

    // Calculate meter heights
    float reductionHeight = juce::jmap(smoothedReduction_,
                                        minReductionDB_, maxReductionDB_,
                                        0.0f, height * 0.4f);
    reductionHeight = juce::jlimit(0.0f, height * 0.4f, reductionHeight);

    float peakHeight = juce::jmap(smoothedPeak_,
                                   -60.0f, 0.0f,
                                   0.0f, height);
    peakHeight = juce::jlimit(0.0f, height, peakHeight);

    // Draw reduction meter (green to red)
    auto reductionArea = bounds.removeFromTop(height * 0.4f);
    float reductionY = reductionArea.getBottom() - reductionHeight;

    g.setColour(juce::Colours::green);
    g.fillRect(reductionArea.getX(), reductionY, width, reductionHeight * 0.3f);

    g.setColour(juce::Colours::yellow);
    g.fillRect(reductionArea.getX(), reductionY + reductionHeight * 0.3f,
               width, reductionHeight * 0.3f);

    g.setColour(juce::Colours::red);
    g.fillRect(reductionArea.getX(), reductionY + reductionHeight * 0.6f,
               width, reductionHeight * 0.4f);

    // Draw peak level indicator
    g.setColour(juce::Colours::white);
    g.drawHorizontalLine(peakHeight, bounds.getX(), bounds.getRight());

    // Scale labels
    g.setColour(juce::Colours::grey);
    g.setFont(9.0f);
    g.drawText("0dB", bounds.getRight() - 20, 2, 18, juce::Justification::centred, false);
    g.drawText("-30dB", bounds.getRight() - 20, height * 0.4f - 10, 18, juce::Justification::centred, false);
    g.drawText("-60dB", bounds.getRight() - 20, height - 10, 18, juce::Justification::centred, false);
}

void GainMeter::resized() {
    // Auto-resize not needed
}

}  // namespace DistortionPro
