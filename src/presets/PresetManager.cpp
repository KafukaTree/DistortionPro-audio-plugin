/**
 * PresetManager.cpp
 *
 * Preset management implementation
 */

#include "PresetManager.h"

namespace DistortionPro {

PresetManager::PresetManager() {
}

PresetManager::~PresetManager() {
}

void PresetManager::initialize(const juce::String& userPresetsPath) {
    userPresetsPath_ = userPresetsPath;

    // Ensure user presets directory exists
    juce::File(userPresetsPath_).createDirectory();

    loadAllPresets();
}

void PresetManager::loadAllPresets() {
    presets_.clear();
    presetsByCategory_.clear();

    // Load factory presets
    loadFactoryPresets();

    // Load user presets
    juce::File userDir(userPresetsPath_);
    if (userDir.exists() && userDir.isDirectory()) {
        auto files = userDir.findChildFiles(juce::File::findFiles, false, "*.json");

        for (const auto& file : files) {
            Preset preset;
            if (parsePresetFile(file, preset)) {
                preset.category = "User";
                presets_.push_back(preset);
                presetsByCategory_["User"].push_back(preset);
            }
        }
    }
}

void PresetManager::loadFactoryPresets() {
    Preset p;

    // Vintage Overdrive
    p.name = "Vintage Overdrive";
    p.category = "Factory";
    p.type = DistortionType::Overdrive;
    p.params.drive = 0.4f;
    p.params.tone = 0.6f;
    p.params.output = 0.8f;
    p.params.mix = 1.0f;
    p.params.depth = 0.5f;
    p.params.attack = 0.5f;
    p.params.type = DistortionType::Overdrive;
    p.params.oversample = false;
    presets_.push_back(p);
    presetsByCategory_["Factory"].push_back(p);

    // British Crunch
    p.name = "British Crunch";
    p.category = "Factory";
    p.type = DistortionType::Distortion;
    p.params.drive = 0.55f;
    p.params.tone = 0.5f;
    p.params.output = 0.75f;
    p.params.mix = 1.0f;
    p.params.depth = 0.4f;
    p.params.attack = 0.6f;
    p.params.type = DistortionType::Distortion;
    p.params.oversample = false;
    presets_.push_back(p);
    presetsByCategory_["Factory"].push_back(p);

    // Hard Rock
    p.name = "Hard Rock";
    p.category = "Factory";
    p.type = DistortionType::Distortion;
    p.params.drive = 0.75f;
    p.params.tone = 0.4f;
    p.params.output = 0.7f;
    p.params.mix = 1.0f;
    p.params.depth = 0.3f;
    p.params.attack = 0.7f;
    p.params.type = DistortionType::Distortion;
    p.params.oversample = true;
    presets_.push_back(p);
    presetsByCategory_["Factory"].push_back(p);

    // Fuzzy Math
    p.name = "Fuzzy Math";
    p.category = "Factory";
    p.type = DistortionType::Fuzz;
    p.params.drive = 0.85f;
    p.params.tone = 0.3f;
    p.params.output = 0.65f;
    p.params.mix = 1.0f;
    p.params.depth = 0.6f;
    p.params.attack = 0.3f;
    p.params.type = DistortionType::Fuzz;
    p.params.oversample = true;
    presets_.push_back(p);
    presetsByCategory_["Factory"].push_back(p);

    // Clean Boost
    p.name = "Clean Boost";
    p.category = "Factory";
    p.type = DistortionType::Overdrive;
    p.params.drive = 0.2f;
    p.params.tone = 0.8f;
    p.params.output = 0.9f;
    p.params.mix = 1.0f;
    p.params.depth = 0.8f;
    p.params.attack = 0.9f;
    p.params.type = DistortionType::Overdrive;
    p.params.oversample = false;
    presets_.push_back(p);
    presetsByCategory_["Factory"].push_back(p);

    // Studio Warmth
    p.name = "Studio Warmth";
    p.category = "Factory";
    p.type = DistortionType::Saturation;
    p.params.drive = 0.25f;
    p.params.tone = 0.7f;
    p.params.output = 0.85f;
    p.params.mix = 1.0f;
    p.params.depth = 0.7f;
    p.params.attack = 0.8f;
    p.params.type = DistortionType::Saturation;
    p.params.oversample = false;
    presets_.push_back(p);
    presetsByCategory_["Factory"].push_back(p);
}

std::vector<Preset> PresetManager::getPresetsByCategory(const juce::String& category) const {
    auto it = presetsByCategory_.find(category);
    if (it != presetsByCategory_.end()) {
        return it->second;
    }
    return {};
}

std::vector<juce::String> PresetManager::getCategories() const {
    std::vector<juce::String> categories;
    for (const auto& pair : presetsByCategory_) {
        categories.push_back(pair.first);
    }
    return categories;
}

bool PresetManager::loadPreset(const juce::String& presetName, ProcessorParams& params,
                                 DistortionType& type) {
    for (const auto& preset : presets_) {
        if (preset.name == presetName) {
            params = preset.params;
            type = preset.type;
            return true;
        }
    }
    return false;
}

bool PresetManager::savePreset(const juce::String& presetName, const ProcessorParams& params,
                                DistortionType type, const juce::String& category) {
    // Create preset
    Preset preset;
    preset.name = presetName;
    preset.category = category;
    preset.params = params;
    preset.type = type;

    // Export to file
    juce::String fileName = presetName.replaceCharacters(" ", "_") + ".json";
    juce::File file(juce::File(userPresetsPath_).getChildFile(fileName));

    if (exportPreset(preset, file.getFullPathName())) {
        // Reload all presets
        loadAllPresets();
        return true;
    }
    return false;
}

bool PresetManager::deletePreset(const juce::String& presetName) {
    juce::String fileName = presetName.replaceCharacters(" ", "_") + ".json";
    juce::File file(juce::File(userPresetsPath_).getChildFile(fileName));

    if (file.exists() && file.deleteFile()) {
        loadAllPresets();
        return true;
    }
    return false;
}

bool PresetManager::exportPreset(const Preset& preset, const juce::String& filePath) {
    juce::String json = presetToJson(preset);

    juce::File file(filePath);
    return file.replaceWithText(json);
}

bool PresetManager::importPreset(const juce::String& filePath, Preset& preset) {
    juce::File file(filePath);

    if (!file.exists()) {
        return false;
    }

    return parsePresetFile(file, preset);
}

juce::String PresetManager::presetToJson(const Preset& preset) {
    juce::String typeStr;

    switch (preset.type) {
        case DistortionType::Overdrive:   typeStr = "overdrive"; break;
        case DistortionType::Distortion:  typeStr = "distortion"; break;
        case DistortionType::Fuzz:        typeStr = "fuzz"; break;
        case DistortionType::Saturation:  typeStr = "saturation"; break;
        default:                           typeStr = "overdrive"; break;
    }

    juce::String json = "{\n";
    json += "  \"name\": \"" + preset.name + "\",\n";
    json += "  \"category\": \"" + preset.category + "\",\n";
    json += "  \"type\": \"" + typeStr + "\",\n";
    json += "  \"parameters\": {\n";
    json += "    \"drive\": " + juce::String(preset.params.drive, 4) + ",\n";
    json += "    \"tone\": " + juce::String(preset.params.tone, 4) + ",\n";
    json += "    \"output\": " + juce::String(preset.params.output, 4) + ",\n";
    json += "    \"mix\": " + juce::String(preset.params.mix, 4) + ",\n";
    json += "    \"depth\": " + juce::String(preset.params.depth, 4) + ",\n";
    json += "    \"attack\": " + juce::String(preset.params.attack, 4) + "\n";
    json += "  }\n";
    json += "}";

    return json;
}

bool PresetManager::presetFromJson(const juce::String& json, Preset& preset) {
    // Simple JSON parsing (can use juce::JSON::parse for more robust handling)
    // This is a simplified implementation

    return false;  // Use parsePresetFile instead
}

bool PresetManager::parsePresetFile(const juce::File& file, Preset& preset) {
    if (!file.exists()) {
        return false;
    }

    // Parse using JUCE JSON - JUCE 7 API
    auto parsed = juce::JSON::parse(file);

    if (parsed.isObject()) {
        juce::DynamicObject* obj = parsed.getDynamicObject();

        preset.name = obj->getProperty("name").toString();
        preset.category = obj->getProperty("category").toString();

        juce::String typeStr = obj->getProperty("type").toString();
        if (typeStr == "overdrive") {
            preset.type = DistortionType::Overdrive;
        } else if (typeStr == "distortion") {
            preset.type = DistortionType::Distortion;
        } else if (typeStr == "fuzz") {
            preset.type = DistortionType::Fuzz;
        } else if (typeStr == "saturation") {
            preset.type = DistortionType::Saturation;
        }

        // Parse parameters
        auto paramsVar = obj->getProperty("parameters");
        if (paramsVar.isObject()) {
            auto paramsObj = paramsVar.getDynamicObject();
            preset.params.drive = paramsObj->getProperty("drive");
            preset.params.tone = paramsObj->getProperty("tone");
            preset.params.output = paramsObj->getProperty("output");
            preset.params.mix = paramsObj->getProperty("mix");
            preset.params.depth = paramsObj->getProperty("depth");
            preset.params.attack = paramsObj->getProperty("attack");
        }

        return true;
    }

    return false;
}

juce::String PresetManager::getFactoryPresetsPath() {
    // Return path to built-in factory presets
    return {};
}

}  // namespace DistortionPro
