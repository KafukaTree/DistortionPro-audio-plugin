/**
 * DistortionPro.cpp
 *
 * Main JUCE plugin implementation - JUCE 7 compatible
 */

#include "DistortionPro.h"

namespace DistortionPro {

// Plugin identification
static const juce::String manufacturerName = "DistortionPro";
static const juce::String pluginName = "DistortionPro";

// Parameter IDs
static const juce::String paramDriveId = "drive";
static const juce::String paramToneId = "tone";
static const juce::String paramOutputId = "output";
static const juce::String paramMixId = "mix";
static const juce::String paramDepthId = "depth";
static const juce::String paramAttackId = "attack";
static const juce::String paramTypeId = "type";
static const juce::String paramOversampleId = "oversample";

// Parameter range helpers
static constexpr int getNumDistortionTypes() { return 4; }
static constexpr int getNumPrograms() { return 6; }

//==============================================================================
DistortionPro::DistortionPro() {
    // Initialize programs
    initializePrograms();

    // Create value tree state with parameters
    createParameters();

    // Initialize processor
    processor_.initialize(getSampleRate(), 512);
}

DistortionPro::~DistortionPro() {
}

//==============================================================================
void DistortionPro::initializePrograms() {
    programs_.clear();

    // Factory presets
    ProgramData p;

    p.name = "Vintage Overdrive";
    p.params.drive = 0.4f;
    p.params.tone = 0.6f;
    p.params.output = 0.8f;
    p.params.mix = 1.0f;
    p.params.depth = 0.5f;
    p.params.attack = 0.5f;
    p.params.type = DistortionType::Overdrive;
    p.params.oversample = false;
    programs_.push_back(p);

    p.name = "British Crunch";
    p.params.drive = 0.55f;
    p.params.tone = 0.5f;
    p.params.output = 0.75f;
    p.params.mix = 1.0f;
    p.params.depth = 0.4f;
    p.params.attack = 0.6f;
    p.params.type = DistortionType::Distortion;
    p.params.oversample = false;
    programs_.push_back(p);

    p.name = "Hard Rock";
    p.params.drive = 0.75f;
    p.params.tone = 0.4f;
    p.params.output = 0.7f;
    p.params.mix = 1.0f;
    p.params.depth = 0.3f;
    p.params.attack = 0.7f;
    p.params.type = DistortionType::Distortion;
    p.params.oversample = true;
    programs_.push_back(p);

    p.name = "Fuzzy Math";
    p.params.drive = 0.85f;
    p.params.tone = 0.3f;
    p.params.output = 0.65f;
    p.params.mix = 1.0f;
    p.params.depth = 0.6f;
    p.params.attack = 0.3f;
    p.params.type = DistortionType::Fuzz;
    p.params.oversample = true;
    programs_.push_back(p);

    p.name = "Clean Boost";
    p.params.drive = 0.2f;
    p.params.tone = 0.8f;
    p.params.output = 0.9f;
    p.params.mix = 1.0f;
    p.params.depth = 0.8f;
    p.params.attack = 0.9f;
    p.params.type = DistortionType::Overdrive;
    p.params.oversample = false;
    programs_.push_back(p);

    p.name = "Studio Warmth";
    p.params.drive = 0.25f;
    p.params.tone = 0.7f;
    p.params.output = 0.85f;
    p.params.mix = 1.0f;
    p.params.depth = 0.7f;
    p.params.attack = 0.8f;
    p.params.type = DistortionType::Saturation;
    p.params.oversample = false;
    programs_.push_back(p);

    currentProgram_ = 0;
}

void DistortionPro::createParameters() {
    // Create AudioProcessorValueTreeState
    valueTreeState_ = std::make_unique<juce::AudioProcessorValueTreeState>(
        *this, nullptr, "DISTORTIONPRO", createParameterLayout());
}

//==============================================================================
void DistortionPro::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) {
    processor_.initialize(sampleRate, maximumExpectedSamplesPerBlock);
}

void DistortionPro::releaseResources() {
    processor_.reset();
}

void DistortionPro::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    // Sync parameters from ValueTreeState to DistortionProcessor
    processor_.setParameter(ParameterID::Drive, valueTreeState_->getParameter(paramDriveId)->getValue());
    processor_.setParameter(ParameterID::Tone, valueTreeState_->getParameter(paramToneId)->getValue());
    processor_.setParameter(ParameterID::Output, valueTreeState_->getParameter(paramOutputId)->getValue());
    processor_.setParameter(ParameterID::Mix, valueTreeState_->getParameter(paramMixId)->getValue());
    processor_.setParameter(ParameterID::Depth, valueTreeState_->getParameter(paramDepthId)->getValue());
    processor_.setParameter(ParameterID::Attack, valueTreeState_->getParameter(paramAttackId)->getValue());
    processor_.setParameter(ParameterID::Oversample, valueTreeState_->getParameter(paramOversampleId)->getValue());

    // Sync distortion type from AudioParameterChoice
    auto typeChoice = valueTreeState_->getParameter(paramTypeId);
    if (typeChoice != nullptr) {
        int typeIndex = static_cast<int>(typeChoice->getValue());
        DistortionType type = static_cast<DistortionType>(typeIndex);
        processor_.setDistortionType(type);
    }

    processor_.process(buffer);
}

//==============================================================================
bool DistortionPro::hasEditor() const {
    return true;
}

juce::AudioProcessorEditor* DistortionPro::createEditor() {
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
const juce::String DistortionPro::getName() const {
    return pluginName;
}

bool DistortionPro::acceptsMidi() const {
    return false;
}

bool DistortionPro::producesMidi() const {
    return false;
}

bool DistortionPro::isMidiEffect() const {
    return false;
}

double DistortionPro::getTailLengthSeconds() const {
    return 0.0;
}

//==============================================================================
int DistortionPro::getNumPrograms() {
    return static_cast<int>(programs_.size());
}

int DistortionPro::getCurrentProgram() {
    return currentProgram_;
}

void DistortionPro::setCurrentProgram(int index) {
    if (index >= 0 && index < getNumPrograms()) {
        currentProgram_ = index;
        applyProgram(index);
    }
}

const juce::String DistortionPro::getProgramName(int index) {
    if (index >= 0 && index < getNumPrograms()) {
        return programs_[index].name;
    }
    return {};
}

void DistortionPro::changeProgramName(int index, const juce::String& newName) {
    if (index >= 0 && index < getNumPrograms()) {
        programs_[index].name = newName;
    }
}

//==============================================================================
void DistortionPro::getStateInformation(juce::MemoryBlock& destData) {
    auto state = valueTreeState_->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->setAttribute("currentProgram", currentProgram_);
    copyXmlToBinary(*xml, destData);
}

void DistortionPro::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml && xml->hasTagName(valueTreeState_->state.getType())) {
        valueTreeState_->replaceState(juce::ValueTree::fromXml(*xml));

        if (xml->hasAttribute("currentProgram")) {
            currentProgram_ = xml->getIntAttribute("currentProgram");
            applyProgram(currentProgram_);
        }
    }
}

//==============================================================================
void DistortionPro::applyProgram(int programIndex) {
    if (programIndex >= 0 && programIndex < getNumPrograms()) {
        const auto& prog = programs_[programIndex];

        // Update parameters
        valueTreeState_->getParameter(paramDriveId)->setValueNotifyingHost(prog.params.drive);
        valueTreeState_->getParameter(paramToneId)->setValueNotifyingHost(prog.params.tone);
        valueTreeState_->getParameter(paramOutputId)->setValueNotifyingHost(prog.params.output);
        valueTreeState_->getParameter(paramMixId)->setValueNotifyingHost(prog.params.mix);
        valueTreeState_->getParameter(paramDepthId)->setValueNotifyingHost(prog.params.depth);
        valueTreeState_->getParameter(paramAttackId)->setValueNotifyingHost(prog.params.attack);
        valueTreeState_->getParameter(paramOversampleId)->setValueNotifyingHost(prog.params.oversample ? 1.0f : 0.0f);

        // Set distortion type
        processor_.setDistortionType(prog.params.type);
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout DistortionPro::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Drive
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        paramDriveId, "Drive",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    // Tone
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        paramToneId, "Tone",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    // Output
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        paramOutputId, "Output",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.75f));

    // Mix
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        paramMixId, "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));

    // Depth
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        paramDepthId, "Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    // Attack
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        paramAttackId, "Attack",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    // Distortion Type
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        paramTypeId, "Type",
        juce::StringArray{"Overdrive", "Distortion", "Fuzz", "Saturation"}, 0));

    // Oversample
    layout.add(std::make_unique<juce::AudioParameterBool>(
        paramOversampleId, "Oversample", false));

    return layout;
}

//==============================================================================
}  // namespace DistortionPro
