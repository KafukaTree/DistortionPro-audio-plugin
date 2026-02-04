/**
 * PresetManager.h
 *
 * Manages preset loading, saving, and organization
 */

#pragma once

#include <juce_core/juce_core.h>
#include "../dsp/DistortionProcessor.h"
#include <vector>
#include <map>

namespace DistortionPro {

struct Preset {
    juce::String name;
    juce::String category;
    ProcessorParams params;
    DistortionType type;
};

/**
 * Preset manager class
 */
class PresetManager {
public:
    PresetManager();
    ~PresetManager();

    /**
     * Initialize the preset manager
     * @param userPresetsPath Path to user presets directory
     */
    void initialize(const juce::String& userPresetsPath);

    /**
     * Load all available presets (factory + user)
     */
    void loadAllPresets();

    /**
     * Get all presets
     */
    const std::vector<Preset>& getAllPresets() const { return presets_; }

    /**
     * Get presets by category
     */
    std::vector<Preset> getPresetsByCategory(const juce::String& category) const;

    /**
     * Get available categories
     */
    std::vector<juce::String> getCategories() const;

    /**
     * Load a preset by name
     */
    bool loadPreset(const juce::String& presetName, ProcessorParams& params, DistortionType& type);

    /**
     * Save current settings as a preset
     */
    bool savePreset(const juce::String& presetName, const ProcessorParams& params,
                    DistortionType type, const juce::String& category = "User");

    /**
     * Delete a user preset
     */
    bool deletePreset(const juce::String& presetName);

    /**
     * Export preset to JSON file
     */
    bool exportPreset(const Preset& preset, const juce::String& filePath);

    /**
     * Import preset from JSON file
     */
    bool importPreset(const juce::String& filePath, Preset& preset);

    /**
     * Get preset as JSON string
     */
    juce::String presetToJson(const Preset& preset);

    /**
     * Parse preset from JSON string
     */
    bool presetFromJson(const juce::String& json, Preset& preset);

private:
    juce::String userPresetsPath_;
    std::vector<Preset> presets_;
    std::map<juce::String, std::vector<Preset>> presetsByCategory_;

    // Factory presets
    void loadFactoryPresets();

    // Parse JSON file to preset
    bool parsePresetFile(const juce::File& file, Preset& preset);

    // Get system presets directory
    static juce::String getFactoryPresetsPath();
};

}  // namespace DistortionPro
